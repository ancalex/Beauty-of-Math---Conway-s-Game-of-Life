#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include <FastLED.h>

//gradients
// Gradient palette "Abstract_3_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ggr/tn/Abstract_3.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.
DEFINE_GRADIENT_PALETTE( abstract_3_gp ) {
    0,   0, 38,  1,
   12,  42, 75,  8,
  111, 255,125, 30,
  125,  55, 52, 54,
  150,   1, 12, 87,
  168,  14, 40, 71,
  203,  54, 90, 56,
  224, 130,161, 45,
  240, 249,255, 34,
  245,  84,213,114,
  249,  12,175,255,
  252,   2,213, 45,
  255,   0,255,  0};
// Gradient palette "rainbowsherbet_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/icecream/tn/rainbowsherbet.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
DEFINE_GRADIENT_PALETTE( rainbowsherbet_gp ) {
    0, 255, 33,  4,
   43, 255, 68, 25,
   86, 255,  7, 25,
  127, 255, 82,103,
  170, 255,255,242,
  209,  42,255, 22,
  255,  87,255, 65};
// Gradient palette "grayscale09_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/gray/tn/grayscale09.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.
DEFINE_GRADIENT_PALETTE( grayscale09_gp ) {
    0,   3,  7,  4,
  127, 100,  3,  1,
  255, 255,136,125};
// Gradient palette "Gummy_Kids_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/icons/tn/Gummy-Kids.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.
DEFINE_GRADIENT_PALETTE( Gummy_Kids_gp ) {
	0,   8, 47,  5,
	31,  77,122,  6,
	63, 249,237,  7,
	95, 232, 51,  1,
	127, 215,  0,  1,
	159,  47,  1,  3,
	191,   1,  7, 16,
	223,  52, 22,  6,
	255, 239, 45,  1};
// Gradient palette "ib11_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general/tn/ib11.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.
DEFINE_GRADIENT_PALETTE( ib11_gp ) {
    0, 252, 22,  0,
   42, 255,100,  1,
   53, 255,244,  1,
   99,  42,191, 45,
  119,   0,144,247,
  186,  91,  1,199,
  219, 247,  1,  0,
  255,  34,255,  1};
// Gradient palette "xmas_19_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/xmas/tn/xmas_19.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
DEFINE_GRADIENT_PALETTE( xmas_19_gp ) {
    0,   4, 26,  2,
   51,  55,  1,  1,
  101,   1,  7,  1,
  170, 118,  2,  1,
  216,   4, 40,  6,
  255,  82,  2,  2};
// Gradient palette "pastel_rainbow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/othr/tn/pastel-rainbow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.
DEFINE_GRADIENT_PALETTE( pastel_rainbow_gp ) {
    0,   0,  0,  0,
   33,   1,  2,  8,
   67,   7, 12, 45,
   88,  27, 18, 31,
  110,  67, 27, 19,
  129,  83, 38, 52,
  147, 100, 53,103,
  168,  90, 96, 93,
  189,  79,156, 83,
  206, 110,178,132,
  222, 148,203,197,
  238, 197,227,223,
  255, 255,255,255};
// Gradient palette "fire_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/elem/tn/fire.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
DEFINE_GRADIENT_PALETTE( fire_gp ) {
    0,   1,  1,  0,
   76,  32,  5,  0,
  146, 192, 24,  0,
  197, 220,105,  5,
  240, 252,255, 31,
  250, 252,255,111,
  255, 255,255,255};

const uint8_t MatrixWidth = 16;
const uint8_t MatrixHeight = 16;
const uint8_t mMatrixWidth = 8;
const uint8_t mMatrixHeight = 8;
const uint8_t bMatrixWidth = 2;
const uint8_t bMatrixHeight = 2;
unsigned long density = 45;
int generation = 0;
int fading_step = 6;

#define NUM_LEDS (256)
CRGB leds[NUM_LEDS];
#define LED_PIN  2
#define COLOR_ORDER GRB
#define CHIPSET WS2811
#define BRIGHTNESS 80
uint8_t fullbrightness = 120;
CRGBPalette16 currentPalette = fire_gp;

class Cell {
public:
	bool alive =  1;
	bool prev =  1;
	uint8_t color_index = 0;
};

Cell world[MatrixWidth][MatrixHeight];

void randomFillWorld() {
	for (int i = 0; i < MatrixWidth; i++) {
		for (int j = 0; j < MatrixHeight; j++) {
			if (random(100) < (unsigned long)density) {
				world[i][j].alive = 1;
				world[i][j].color_index = 0;
			}
			else {
				world[i][j].alive = 0;
			}
			world[i][j].prev = world[i][j].alive;
		}
	}
}
//
int neighbours(int x, int y) {
	return (world[(x + 1) % MatrixWidth][y].prev) +
			(world[x][(y + 1) % MatrixHeight].prev) +
			(world[(x + MatrixWidth - 1) % MatrixWidth][y].prev) +
			(world[x][(y + MatrixHeight - 1) % MatrixHeight].prev) +
			(world[(x + 1) % MatrixWidth][(y + 1) % MatrixHeight].prev) +
			(world[(x + MatrixWidth - 1) % MatrixWidth][(y + 1) % MatrixHeight].prev) +
			(world[(x + MatrixWidth - 1) % MatrixWidth][(y + MatrixHeight - 1) % MatrixHeight].prev) +
			(world[(x + 1) % MatrixWidth][(y + MatrixHeight - 1) % MatrixHeight].prev);
}

uint16_t XY( uint8_t x, uint8_t y)
{
	uint16_t i;
	int b = (((x/mMatrixWidth)*bMatrixWidth) + (y/mMatrixHeight))*(mMatrixWidth*mMatrixHeight);
	i = (b+((y%mMatrixHeight)*mMatrixWidth)+(x%mMatrixWidth));
	return i;
}

void chooseNewPalette() {
	switch(random(0, 8)) {
	case 0:
		currentPalette = abstract_3_gp;
		break;

	case 1:
		currentPalette = rainbowsherbet_gp;
		break;

	case 2:
		currentPalette = grayscale09_gp;
		break;

	case 3:
		currentPalette = Gummy_Kids_gp;
		break;

	case 4:
		currentPalette = ib11_gp;
		break;

	case 5:
		currentPalette = xmas_19_gp;
		break;

	case 6:
		currentPalette = pastel_rainbow_gp;
		break;

	case 7:
	default:
		currentPalette = fire_gp;
		break;
	}
}

//The setup function is called once at startup of the sketch
void setup()
{
	// Add your initialization code here
	randomSeed(analogRead(3));
	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setDither(fullbrightness < 255);
			//.setCorrection(TypicalLEDStrip);
	FastLED.setBrightness( BRIGHTNESS );
	Serial.begin(9600);
}

// The loop function is called in an endless loop
void loop()
{
	//Add your repeated code here

	if (generation == 0) {
		fill_solid((CRGB*)leds, NUM_LEDS, CRGB::Black);
		randomFillWorld();
		chooseNewPalette();
	}

	// Display current generation
	for (int i = 0; i < MatrixWidth; i++) {
		for (int j = 0; j < MatrixHeight; j++) {
			if (world[i][j].alive == 1) {
					leds[XY(i, j)] = ColorFromPalette(currentPalette, world[i][j].color_index);
			}
		}
	}

	for (int k = 0; k < BRIGHTNESS; k = k + fading_step) {
		for (int i = 0; i < MatrixWidth; i++) {
			for (int j = 0; j < MatrixHeight; j++) {
				if ( world[i][j].alive == 0){
					leds[XY(i, j)].fadeToBlackBy(k);
				}
			}
		}
		FastLED.show();
	}

    // Birth and death cycle - Conway's Game of Life, density 50, fading_step 6
	for (int x = 0; x < MatrixWidth; x++) {
		for (int y = 0; y < MatrixHeight; y++) {
			// Default is for cell to stay the same
			if (world[x][y].prev == 0){
				world[x][y].color_index += 1;
			}
			int ncount = neighbours(x, y);
			if ((ncount == 3) && world[x][y].prev == 0 ) {
				// A new cell is born
				world[x][y].alive = 1;
				world[x][y].color_index += 2;
			}
			else if (((ncount < 2) || (ncount > 3)) && world[x][y].prev == 1) {
				// Cell dies
				world[x][y].alive = 0;
			}
		}
	}

//	// Birth and death cycle - Amoeba, density 50, fading_step 6
//	for (int x = 0; x < MatrixWidth; x++) {
//		for (int y = 0; y < MatrixHeight; y++) {
//			// Default is for cell to stay the same
//			if (world[x][y].prev == 0){
//				world[x][y].color_index += 1;
//			}
//			int ncount = neighbours(x, y);
//			if ((ncount == 3 || (ncount == 5) || (ncount == 7)) && world[x][y].prev == 0 ) {
//				// A new cell is born
//				world[x][y].alive = 1;
//				world[x][y].color_index += 2;
//			}
//			else if (((ncount == 2) || (ncount == 4) || (ncount == 6) || (ncount == 7)) && world[x][y].prev == 1) {
//				// Cell dies
//				world[x][y].alive = 0;
//			}
//		}
//	}
//
//	// Birth and death cycle - Assimilation, density 25, fading_step 6
//	for (int x = 0; x < MatrixWidth; x++) {
//		for (int y = 0; y < MatrixHeight; y++) {
//			// Default is for cell to stay the same
//			if (world[x][y].prev == 0){
//				world[x][y].color_index += 1;
//			}
//			int ncount = neighbours(x, y);
//			if ((ncount == 3 || (ncount == 4) || (ncount == 5)) && world[x][y].prev == 0 ) {
//				// A new cell is born
//				world[x][y].alive = 1;
//				world[x][y].color_index += 2;
//			}
//			else if (((ncount == 1) || (ncount == 2) || (ncount == 3) || (ncount == 8)) && world[x][y].prev == 1) {
//				// Cell dies
//				world[x][y].alive = 0;
//			}
//		}
//	}
//
//	// Birth and death cycle - Coral, density 50, fading_step 6
//	for (int x = 0; x < MatrixWidth; x++) {
//		for (int y = 0; y < MatrixHeight; y++) {
//			// Default is for cell to stay the same
//			if (world[x][y].prev == 0){
//				world[x][y].color_index += 1;
//			}
//			int ncount = neighbours(x, y);
//			if ((ncount == 3 ) && world[x][y].prev == 0 ) {
//				// A new cell is born
//				world[x][y].alive = 1;
//				world[x][y].color_index += 2;
//			}
//			else if (((ncount == 1) || (ncount == 2) || (ncount == 3)) && world[x][y].prev == 1) {
//				// Cell dies
//				world[x][y].alive = 0;
//			}
//		}
//	}
//
//	// Birth and death cycle - HighLife, density 50, fading_step 6
//	for (int x = 0; x < MatrixWidth; x++) {
//		for (int y = 0; y < MatrixHeight; y++) {
//			// Default is for cell to stay the same
//			if (world[x][y].prev == 0){
//				world[x][y].color_index += 1;
//			}
//			int ncount = neighbours(x, y);
//			if ((ncount == 3 || ncount == 6) && world[x][y].prev == 0 ) {
//				// A new cell is born
//				world[x][y].alive = 1;
//				world[x][y].color_index += 2;
//			}
//			else if ((ncount < 2 || ncount > 3) && world[x][y].prev == 1) {
//				// Cell dies
//				world[x][y].alive = 0;
//			}
//		}
//	}
//
//	// Birth and death cycle - Gnarl, density 8, fading_step 12
//	for (int x = 0; x < MatrixWidth; x++) {
//		for (int y = 0; y < MatrixHeight; y++) {
//			// Default is for cell to stay the same
//			if (world[x][y].prev == 0){
//				world[x][y].color_index += 1;
//			}
//			int ncount = neighbours(x, y);
//			if ((ncount == 1) && world[x][y].prev == 0 ) {
//				// A new cell is born
//				world[x][y].alive = 1;
//				world[x][y].color_index += 2;
//			}
//			else if ((ncount < 1 || ncount > 1) && world[x][y].prev == 1) {
//				// Cell dies
//				world[x][y].alive = 0;
//			}
//		}
//	}
//
//	// Birth and death cycle - LongLife, density 50, fading_step 6
//	for (int x = 0; x < MatrixWidth; x++) {
//		for (int y = 0; y < MatrixHeight; y++) {
//			// Default is for cell to stay the same
//			if (world[x][y].prev == 0){
//				world[x][y].color_index += 1;
//			}
//			int ncount = neighbours(x, y);
//			if ((ncount == 3 || ncount == 4 || ncount == 5) && world[x][y].prev == 0 ) {
//				// A new cell is born
//				world[x][y].alive = 1;
//				world[x][y].color_index += 2;
//			}
//			else if ((ncount < 5 || ncount > 5) && world[x][y].prev == 1) {
//				// Cell dies
//				world[x][y].alive = 0;
//			}
//		}
//	}
//
//	// Birth and death cycle - Replicator, density 2, fading_step 8
//	for (int x = 0; x < MatrixWidth; x++) {
//		for (int y = 0; y < MatrixHeight; y++) {
//			// Default is for cell to stay the same
//			if (world[x][y].prev == 0){
//				world[x][y].color_index += 1;
//			}
//			int ncount = neighbours(x, y);
//			if ((ncount == 1 || ncount == 3 || ncount == 5 || ncount == 7) && world[x][y].prev == 0 ) {
//				// A new cell is born
//				world[x][y].alive = 1;
//				world[x][y].color_index += 2;
//			}
//			else if ((ncount = 2 || ncount == 4 || ncount == 6 || ncount == 8) && world[x][y].prev == 1) {
//				// Cell dies
//				world[x][y].alive = 0;
//			}
//		}
//	}

	// Copy next generation into place
	for (int x = 0; x < MatrixWidth; x++) {
		for (int y = 0; y < MatrixHeight; y++) {
			world[x][y].prev = world[x][y].alive;
		}
	}

	generation++;
	if (generation >= 255) {
		generation = 0;
	}
}
