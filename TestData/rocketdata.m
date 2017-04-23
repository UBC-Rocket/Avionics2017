data = csvread('test_rocketdata.csv', 1, 0);
times = data(:, 1);
gyros = data(:, 2);
accels = data(:, 3);
mags = data(:, 4);
alts = data(:, 5);
temps = data(:, 6);

length = size(times, 1);
%length = 500;

outFile = fopen('MPUData.h', 'w');

fprintf(outFile, '#define SIM_LENGTH %d\n\n', length);

fprintf(outFile, 'static PROGMEM unsigned int times[%d] = {', length);
fprintf(outFile, '%u, ', times(1:length));
fprintf(outFile, '};\n');


fprintf(outFile, 'static PROGMEM short gyros[%d] = {', length);
fprintf(outFile, '%d, ', gyros(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'static PROGMEM short accels[%d] = {', length);
fprintf(outFile, '%d, ', accels(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'static PROGMEM short mags[%d] = {', length);
fprintf(outFile, '%d, ', mags(1:length));
fprintf(outFile, '};\n');

fclose(outFile);

outFile = fopen('MPLData.h', 'w');

fprintf(outFile, '#define SIM_LENGTH %d\n\n', length);

fprintf(outFile, 'static PROGMEM unsigned int times[%d] = {', length);
fprintf(outFile, '%u, ', times(1:length));
fprintf(outFile, '};\n');


fprintf(outFile, 'static PROGMEM float alts[%d] = {', length);
fprintf(outFile, '%f, ', alts(1:length));
fprintf(outFile, '};\n');

fprintf(outFile, 'static PROGMEM float temps[%d] = {', length);
fprintf(outFile, '%f, ', temps(1:length));
fprintf(outFile, '};\n');

fclose(outFile);
