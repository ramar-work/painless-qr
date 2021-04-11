/* ------------------------------------------------------- *
 * pqr.c
 * ====
 * 
 * Easy to use QR to SVG generator...
 * 
 * ------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrcodegen.h>

#define PROG "pqr"

#define HELP \
	"-a, --address <arg>    Specify an address (or text) for a new QR code.\n" \
	"-s, --size <arg>       Specify _numeric_ size (in pixels) for the new QR code.\n" \
	"-b, --background <arg> Specify a different background color.\n" \
	"-f, --foreground <arg> Specify a different foreground color.\n" \
	"-i, --image <arg>          Throw an image in\n" \
	"-p, --image-position <arg> Set position of imported image.\n" \
	"    --vomit            Output totally random colors.\n" \
	"    --svg              Dump QR sequence to XML SVG (default).\n" \
	"    --console          Dump QR sequence to console.\n" \
	"-v, --verbose          Be verbose and tell me everything.\n" \
	"-h, --help             Show help.\n"



//Global command-line args
int a_verbose = 0;
int a_console = 0;
int a_svg = 1;
int a_size = 64;
const char *a_address = NULL;
const char *a_fg = "#000000";
const char *a_bg = "#FFFFFF";



//Come up with a nice, evenly divisible number
static int calc_closest_max ( int lbw ) {
	return 0;
}



// Prints the given QR Code to the console.
static void print_console  ( const uint8_t qrcode[] ) {
	int size = qrcodegen_getSize(qrcode);
	int border = 4;
	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
			fputs((qrcodegen_getModule(qrcode, x, y) ? "##" : "  "), stdout);
		}
		fputs("\n", stdout);
	}
	fputs("\n", stdout);
}



// Prints the given QR Code to the console.
static void print_xml (const uint8_t qrcode[]) {

	//Define this stuff early
	FILE * f = stdout;

	//Do we automatically go to millimeters?
	const char headerfmt[] = \
		"<?xml version=\"1.0\" standalone=\"no\"?>\n" \
		"\t<svg width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\"\n" \
		"\t\txmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n" \
		"\t<desc>QR for test</desc>\n"
	;

	const char boxfmt[] = \
		"\t<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"%s\" />\n";


	int size = qrcodegen_getSize(qrcode);
	int sqsize = 10; 
	int border = 0;

	//Dump the header
	fprintf( f, headerfmt, 250, 250, 250, 250);

	//Loop through the rest
	for (int yp = 0, y = -border; y < size + border; y++, yp = y * sqsize ) {
		for (int xp = 0, x = -border; x < size + border; x++, xp = x * sqsize ) {
			const char *color = qrcodegen_getModule(qrcode, x, y) ? a_fg: a_bg;
			fprintf( f, boxfmt, xp, yp, sqsize, sqsize, color );	
		}
	}

	//Print the closing tag
	fprintf( f, "</svg>\n" );

	//Flush, in case we're writing to stdout
	fflush( f );
}



int main ( int argc, char *argv[] ) {
	
	if ( argc < 2 ) {
		fprintf( stderr, PROG ": no arguments specified...\n%s", HELP );
		return 1;
	}

	while ( *argv ) {
		if ( !strcmp( "-a", *argv ) || !strcmp( "--address", *argv ) ) {
			a_address = *( ++argv );	
		}

		if ( !strcmp( "-v", *argv ) || !strcmp( "--verbose", *argv ) ) {
			a_verbose = 1;	
		}

		if ( !strcmp( "--console", *argv ) ) {
			a_console = 1, a_svg = 0;
		}

		if ( !strcmp( "--svg", *argv ) ) {
			a_console = 0, a_svg = 1;
		}

		if ( !strcmp( "-s", *argv ) || !strcmp( "--size", *argv ) ) {
			a_size = atoi( *( ++argv ) );	
		}

		if ( !strcmp( "-f", *argv ) || !strcmp( "--foreground", *argv ) ) {
			a_fg = *( ++argv );	
		}

		if ( !strcmp( "-b", *argv ) || !strcmp( "--background", *argv ) ) {
			a_bg = *( ++argv );	
		}

		if ( !strcmp( "-h", *argv ) || !strcmp( "--help", *argv ) ) {
			fprintf( stderr, PROG ":\n%s\n", HELP );
			return 1;
		}

		argv++;
	}

	if ( a_verbose && a_address ) {
		fprintf( stderr, "Encoding %s\n", a_address );
	}

	if ( !a_address ) {
		fprintf( stderr, PROG ": no address specified.\n" );
		return 1;
	}


	//Do the qr transition.
	unsigned char qrcode[ qrcodegen_BUFFER_LEN_MAX ] = { 0 };
	unsigned char tmp[ qrcodegen_BUFFER_LEN_MAX ] = { 0 };
	enum qrcodegen_Ecc qr_ec_level = qrcodegen_Ecc_LOW;

	int ok = qrcodegen_encodeText( a_address, tmp, qrcode, qr_ec_level,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true );

	if ( a_console && ok ) { 
		print_console( qrcode );
	}

	if ( a_svg && ok ) {
		print_xml( qrcode );
	}
	
	return 0;
}
