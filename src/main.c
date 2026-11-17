#include <stdio.h>

#include <unistd.h>
#include <pigpio.h>


#define PIN 17


int main(int argc, char* argv[])
{
float minTemp = 68.0;
float maxTemp = 76.0;
float temp;
float total = 0.0;
int count = 0;
int choice = 0;
FILE* logFile;
FILE* statusFile;


gpioInitialise();
gpioSetMode(PIN, PI_OUTPUT);
gpioWrite(PIN, 0);


logFile = fopen("temperature_log.csv", "a");
if (logFile == NULL)
{
printf("Could not open the log file.\n");
gpioTerminate();
return 1;
}



while (choice != 4)
{
printf("\n=== Smart Classroom Comfort Monitor ===\n");
printf("Acceptable range: %.1f F - %.1f F\n", minTemp, maxTemp);
printf("1. Enter a temperature\n");
printf("2. Show average temperature\n");
printf("3. Change acceptable range\n");
printf("4. Exit\n");
printf("Choice: ");
scanf("%d", &choice);


if (choice == 1)
{
printf("Enter temperature (F): ");
scanf("%f", &temp);


statusFile = fopen("status.txt", "w");


if (temp >= minTemp && temp <= maxTemp)
{
gpioWrite(PIN, 0);
printf("%.1f F -> NORMAL (LED OFF)\n", temp);
fprintf(logFile, "%.1f,NORMAL\n", temp);
fprintf(statusFile, "NORMAL\n");
}
else
{
gpioWrite(PIN, 1); 
printf("%.1f F -> WARNING (LED ON)\n", temp);
fprintf(logFile, "%.1f,WARNING\n", temp);
fprintf(statusFile, "WARNING\n");
}


fclose(statusFile);
total = total + temp;
count = count + 1;
}
else if (choice == 2)
{
if (count == 0)
{
printf("No temperatures recorded yet.\n");
}
else
{
printf("Average of %d readings: %.2f F\n", count, total / count);
}
}
else if (choice == 3)
{
float newMin;
float newMax;
printf("Enter new minimum (F): ");
scanf("%f", &newMin);
printf("Enter new maximum (F): ");
scanf("%f", &newMax);


if (newMin < newMax)
{
minTemp = newMin;
maxTemp = newMax;
printf("Range updated to %.1f F - %.1f F\n", minTemp, maxTemp);
}
else
{
printf("Minimum must be less than maximum. Range not changed.\n");
}
}
else if (choice != 4)
{
printf("Please enter 1, 2, 3, or 4.\n");
}
}


gpioWrite(PIN, 0);
fclose(logFile);
gpioTerminate();
printf("Goodbye!\n");
return 0;
}
