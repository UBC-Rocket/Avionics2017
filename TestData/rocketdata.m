data = csvread('test_rocketdata.csv', 1, 0);
times = data(:, 1);
gyros = data(:, 2);
accels = data(:, 3);
mags = data(:, 4);
alts = data(:, 5);
temps = data(:, 6);

length = size(times, 1);

outFile = fopen('testData.h', 'w');

fprintf(outFile, '#define SIM_LENGTH %d\n\n', length);

fprintf(outFile, 'unsigned long times[%d] = {', length);
fprintf(outFile, '%u, ', times);
fprintf(outFile, '};\n');

fprintf(outFile, 'int gyros[%d] = {', length);
fprintf(outFile, '%d, ', gyros);
fprintf(outFile, '};\n');

fprintf(outFile, 'int accels[%d] = {', length);
fprintf(outFile, '%d, ', accels);
fprintf(outFile, '};\n');

fprintf(outFile, 'int mags[%d] = {', length);
fprintf(outFile, '%d, ', mags);
fprintf(outFile, '};\n');

fprintf(outFile, 'float alts[%d] = {', length);
fprintf(outFile, '%f, ', alts);
fprintf(outFile, '};\n');

fprintf(outFile, 'float temps[%d] = {', length);
fprintf(outFile, '%f, ', temps);
fprintf(outFile, '};\n');

fclose(outFile);
