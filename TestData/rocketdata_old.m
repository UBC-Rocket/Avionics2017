data = csvread('test_rocketdata.csv', 1, 0);
times = data(:, 1);
gyros = data(:, 2);
accels = data(:, 3);
mags = data(:, 4);
alts = data(:, 5);
temps = data(:, 6);

length = size(times, 1);
%length = 500;

outFile = fopen('simData.h', 'w');

fprintf(outFile, '#ifndef SIMDATA_H\n#define SIMDATA_H\n\n');

fprintf(outFile, '#define SIM_LENGTH %d\n\n', length);

fprintf(outFile, 'class simData {\n');

fprintf(outFile, 'const unsigned int times[%d] = {', length);
fprintf(outFile, '%u, ', times(1:length));
fprintf(outFile, '};\n');


fprintf(outFile, 'const short gyros[%d] = {', length);
fprintf(outFile, '%d, ', gyros(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'const short accels[%d] = {', length);
fprintf(outFile, '%d, ', accels(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'const short mags[%d] = {', length);
fprintf(outFile, '%d, ', mags(1:length));
fprintf(outFile, '};\n');


fprintf(outFile, 'const float alts[%d] = {', length);
fprintf(outFile, '%f, ', alts(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'const float temps[%d] = {', length);
fprintf(outFile, '%f, ', temps(1:length));
fprintf(outFile, '};\n');


fprintf(outFile, 'public:\n  static unsigned int getTime(int pos);\n  static short getGyro(int pos);\n  static short getAccel(int pos);\n  static short getMag(int pos);\n  static float getAlt(int pos);\n  static float getTemp(int pos);\n};\n\n#endif');

fclose(outFile);
