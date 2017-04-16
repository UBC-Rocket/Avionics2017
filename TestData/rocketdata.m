data = csvread('test_rocketdata.csv', 1, 0);
times = data(:, 1);
gyros = data(:, 2);
accels = data(:, 3);
mags = data(:, 4);
alts = data(:, 5);
temps = data(:, 6);

length = size(times, 1);
%length = 500;

outFile = fopen('testData.h', 'w');

fprintf(outFile, '#define SIM_LENGTH %d\n\n', length);

fprintf(outFile, 'const PROGMEM unsigned int times[%d] = {', length);
fprintf(outFile, '%u, ', times(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, '#ifdef MPU_H\n');

fprintf(outFile, 'const PROGMEM short gyros[%d] = {', length);
fprintf(outFile, '%d, ', gyros(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'const PROGMEM short accels[%d] = {', length);
fprintf(outFile, '%d, ', accels(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'const PROGMEM short mags[%d] = {', length);
fprintf(outFile, '%d, ', mags(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, '#endif\n\n#ifdef MPL_H\n');

fprintf(outFile, 'const PROGMEM float alts[%d] = {', length);
fprintf(outFile, '%f, ', alts(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'const PROGMEM float temps[%d] = {', length);
fprintf(outFile, '%f, ', temps(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, '#endif');

fclose(outFile);
