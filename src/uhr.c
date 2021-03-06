#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include <unistd.h>


// helpful functions 
void clear ( );
void wait_for ( struct timespec delay );
void print_balken ( char c, int num, char *nformat, int zahl );

//display functions
void simple_display ( struct tm *current_time );
void break_display ( struct tm *current_time );
void side_balken_display ( struct tm *current_time );
void side_balken_fat_display ( struct tm *current_time );
void side_alt_balken_display ( struct tm *current_time );
void timestamp_display ( struct tm *current_time );
void timestamp_break_display ( struct tm *current_time );
void big_display ( struct tm *current_time );
void clock_display ( struct tm *current_time );
#define NUM_DISPLAYS 9

int main ( int argc, char **argv ) {
	struct tm current_time;
	time_t timestamp;
	struct timespec delay = {};

	int i;
	int index = 0;
  int times = 0;
  int counter = 0;

	char *names[] =  {
		"simple",
		"break",
		"balken",
		"balken_fat",
		"balken_alt",
		"timestamp",
		"timestamp_break",
		"big",
	  "clock"
	};

	void (*funcp[])(struct tm *) = {
		simple_display,
		break_display,
		side_balken_display,
		side_balken_fat_display,
		side_alt_balken_display,
		timestamp_display,
		timestamp_break_display,
		big_display,
		clock_display
	};

  int opt;

	//parsing arguments using getopt
  while((opt = getopt(argc, argv, "hln:")) != -1) {
    switch(opt) {
      case 'n':
        // how many times the clock is refreshed (0 -> endless)
        times = atoi(optarg);
        break;
      case 'l':
        // this lists out all the styles
        printf("possible %i display styles: \n", NUM_DISPLAYS);
				for ( i = 0; i < NUM_DISPLAYS; i++ ) {
					printf ("   - %s\n", names[i]);
				}
				exit(EXIT_SUCCESS);
      case 'h':
      default:
        puts("This programm displays a clock on the screen in diffrent styles. Use option -l");
        puts("to list them all out." );
				printf("USAGE: %s [-l] [-n times] [-h] [display_style]\n\n", argv[0]);
				puts("This awesome programm is written by Mogria.");
				exit(EXIT_FAILURE);
    }
  }
	if ( optind < argc ) {
    for ( i = 0; i < NUM_DISPLAYS; i++) {
      if ( strcmp ( names[i], argv[optind] ) == 0 ) {
        index = i;
      }
    }
	}

  // main loop
	while ( times == 0 || counter++ < times ) {
		// get the current time
		time(&timestamp);
		current_time = *localtime(&timestamp);

		// displaying
		funcp[index](&current_time);

		// delaying
    if(times == 0 || counter != times) {
      delay.tv_sec = 1;
      wait_for(delay);
    }
	}

	return EXIT_SUCCESS;
}

void clear ( ) {
	puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void wait_for ( struct timespec delay ) {
	struct timespec rest = {};
	while ( nanosleep (&delay, &rest ) == -1 ) {
		delay = rest;
	}
}
void print_balken ( char c, int num, char *nformat, int zahl ) {
	char *p = malloc ( ( num + 1 ) * sizeof(char));
	char format_buf[20];
	memset(p, '#', num * sizeof(char));
	p[num] = '\0';
	snprintf(format_buf, 20, "%%c %s %%s\n", nformat);
	printf(format_buf, c, zahl == -1 ? num : zahl, p);
	free(p);
}

void simple_display ( struct tm *current_time ) {
	printf("\r");
	printf("%02i:%02i:%02i", current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
	fflush(stdout);
}

void break_display ( struct tm *current_time ) {
	printf("%02i:%02i:%02i\n", current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
}

void side_balken_display ( struct tm *current_time ) {
	clear ( );
	print_balken('H', current_time->tm_hour, "%02i", -1);
	print_balken('M', current_time->tm_min, "%02i", -1);
	print_balken('S', current_time->tm_sec, "%02i", -1);
}

void side_balken_fat_display ( struct tm *current_time ) {
	clear ( );
	print_balken('H', current_time->tm_hour, "%i", current_time->tm_hour / 10);
	print_balken('H', current_time->tm_hour, "%i", current_time->tm_hour % 10);
	puts("");
	print_balken('M', current_time->tm_min, "%i", current_time->tm_min / 10);
	print_balken('M', current_time->tm_min, "%i", current_time->tm_min % 10);
	puts("");
	print_balken('S', current_time->tm_sec, "%i", current_time->tm_sec / 10);
	print_balken('S', current_time->tm_sec, "%i", current_time->tm_sec % 10);
}

void side_alt_balken_display ( struct tm *current_time ) {
	clear ( );
	print_balken('H', current_time->tm_hour / 10, "%i", -1);
	print_balken('H', current_time->tm_hour % 10, "%i", -1);
	puts("");
	print_balken('M', current_time->tm_min / 10, "%i", -1);
	print_balken('M', current_time->tm_min % 10, "%i", -1);
	puts("");
	print_balken('S', current_time->tm_sec / 10, "%i", -1);
	print_balken('S', current_time->tm_sec % 10, "%i", -1);
}

void timestamp_display ( struct tm *current_time ) {
	time_t time = mktime ( current_time );
	printf ( "\r%i", (int)time );
	fflush(stdout);
}

void timestamp_break_display ( struct tm *current_time ) {
	time_t time = mktime ( current_time );
	printf ( "%i\n", (int)time );
}

void big_display ( struct tm *current_time ) {

  int offset = 6 * 6;
	char *numbers = malloc( 11 * offset * sizeof(char));

	strncpy(numbers + offset * 0, 
			" ___ \n"
			"/   \\\n"
			"|   |\n"
			"|   |\n"
			"|   |\n"
			"\\___/\n"
	, offset);

	strncpy(numbers + offset * 1,
			"   _ \n"
			"  / |\n"
			" /  |\n"
			"/   |\n"
			"    |\n"
			"    |\n"
	, offset);
	strncpy(numbers + offset * 2,
			" ___ \n"
			"/   \\\n"
			"    /\n"
			"   / \n"
			"  /  \n"
			"_/___\n"
	, offset);
	strncpy(numbers + offset * 3,
			" ___ \n"
			"/   \\\n"
			"    /\n"
			" --< \n"
			"    \\\n"
			"\\___/\n"
	, offset);
	strncpy(numbers + offset * 4,
			"     \n"
			"  /| \n"
			" / | \n"
			"/__|_\n"
			"   | \n"
			"   | \n"
	, offset);
	strncpy(numbers + offset * 5,
			"_____\n"
			"| __ \n"
			"|/  \\\n"
			"    |\n"
			"    |\n"
			"\\___/\n"
	, offset);
	strncpy(numbers + offset * 6,
			" ___ \n"
			"/   \\\n"
			"| __ \n"
			"|/  \\\n"
			"|   |\n"
			"\\___/\n"
	, offset);
	strncpy(numbers + offset * 7,
			"_____\n"
			"    /\n"
			"   / \n"
			" -/- \n"
			" /   \n"
			"/    \n"
	, offset);
	strncpy(numbers + offset * 8,
			" ___ \n"
			"/   \\\n"
			"\\___/\n"
			"/   \\\n"
			"|   |\n"
			"\\___/\n"
	, offset);
	strncpy(numbers + offset * 9,
			" ___ \n"
			"/   \\\n"
			"|   |\n"
			"\\___/\n"
			"    |\n"
			"\\___/\n"
	, offset);

	strncpy(numbers + offset * 10,
			"  _  \n"
			" / \\ \n"
			" \\_/ \n"
			"  _  \n"
			" / \\ \n"
			" \\_/ \n"
	, offset);

  int needed_numbers[8] = {
		current_time->tm_hour / 10,
		current_time->tm_hour % 10,
		10,
		current_time->tm_min / 10,
		current_time->tm_min % 10,
		10,
		current_time->tm_sec / 10,
		current_time->tm_sec % 10 
	};
	
	int i;
	for ( i = 0; i < offset * 11; i++) {
		if(numbers[i] == '\n') {
			numbers[i] = '\0';
		}
	}
	int x;
	int y;
	clear( );
	for ( x = 0; x < 6; x++) {
		for ( y = 0; y < 8; y++ ) {
			printf ( "%s   ", numbers + needed_numbers[y] * offset + x * 6);
		}
		puts ("");
	}
}


#define CSIZE (60)

void clock_set_content(char *clock, double degrees, double length, double radius, char value) {
	int size = round(radius * 2);
	int posx = round(sin(degrees / 180.0 * M_PI) * length + radius);
	int posy = round(cos(degrees / 180.0 * M_PI) * length + radius);
	if(posx >= 0 && posx < size && posy >= 0 && posy < size) {
		*(clock + posx * size + posy) = value;
	}
}
void draw_line(char *clock, double degrees, double radius, char c, double length) {
	int i;
	int size = radius * 2;
	for(i = 0; i < radius * length; i++) {
		clock_set_content(clock, degrees, i, radius, c);
	}
}

void clock_display ( struct tm *current_time) {
	double radius = CSIZE / 2.0;

	char clock[CSIZE][CSIZE] = {};

	int i,x,y;

	int seconds, minutes;

	// fill the whole array with spaces
	memset(clock, ' ', CSIZE * CSIZE);

	// fill the background of the clock
	for(x = 0; x < CSIZE; x++) {
		for(y = 0; y < CSIZE; y++) {
			double dx, dy, distance;
			dx = x /* + 0.5 */;
			dy = y /* + 0.5 */;
			dx =  dx < radius ? radius - dx : dx - radius;
			dy =  dy < radius ? radius - dy : dy - radius;
			distance = sqrt(dx * dx + dy * dy);

			if ( distance < radius ) {
				clock[x][y] = '.';
			}
		}
	}

	// draw the line for the seconds
	draw_line((char *)clock,
			((int)round(seconds = current_time->tm_sec * 6) + 270) % 360,
			radius, 'S', 1);
	// draw the line for the minutes
	draw_line((char *)clock,
			((int)round(minutes = current_time->tm_min * 6 + seconds / 60.0) + 270) % 360,
			radius, 'M', 0.8);
	// draw the line for the hours
	draw_line((char *)clock,
			((int)round(current_time->tm_hour % 12 * 30 + minutes / 12.0) + 270) % 360,
			radius, 'H', 0.5);


	//place this in the middle of the clock
	clock[(int)round(radius)][(int)round(radius)] = 'O';

	// draw the circle of the clock
	for(i = 0; i < 360; i++) {
		clock_set_content((char *)clock, i, radius - 0.5, radius, '#');
	}

	for(i = 0; i < 360; i += 30) {
		clock_set_content((char *)clock, i, radius - 0.5, radius, 'O');
	}

	// clear the screen
	clear();

	//print the new/updated clock
	for(x = 0; x < CSIZE; x++) {
		for(y = 0; y < CSIZE; y++) {
			printf("%c%c", clock[x][y] , clock[x][y] );
		}
		if (x != CSIZE - 1) {
			puts("");
		} else {
			fflush(stdout);
		}
	}
}
