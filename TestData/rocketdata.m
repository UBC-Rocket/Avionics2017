data = csvread('test_rocketdata.csv', 1, 0);
times = data(:, 1);
gyros = data(:, 2);
accels = data(:, 3);
mags = data(:, 4);
alts = data(:, 5);

outFile = fopen('testData.h', 'w');

fprintf(outFile, 'unsigned long times[%d] = {', size(times, 1));
fprintf(outFile, '%u, ', times);
fprintf(outFile, '};\n');

fprintf(outFile, 'int gyros[%d] = {', size(gyros, 1));
fprintf(outFile, '%d, ', gyros);
fprintf(outFile, '};\n');

fprintf(outFile, 'int accels[%d] = {', size(accels, 1));
fprintf(outFile, '%d, ', accels);
fprintf(outFile, '};\n');

fprintf(outFile, 'int mags[%d] = {', size(mags, 1));
fprintf(outFile, '%d, ', mags);
fprintf(outFile, '};\n');

fprintf(outFile, 'float alts[%d] = {', size(alts, 1));
fprintf(outFile, '%f, ', alts);
fprintf(outFile, '};\n');

fclose(outFile);
