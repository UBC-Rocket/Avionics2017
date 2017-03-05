data = csvread('test_rocketdata.csv', 1, 0);
times = data(:, 1);
gyros = data(:, 2);
accels = data(:, 3);
mags = data(:, 4);
alts = data(:, 5);

outFile = fopen('testData.h', 'w');

fprintf(outFile, '#ifndef TESTDATA_H\n#define TESTDATA_H\n\n');

fprintf(outFile, 'unsigned long times[] = {');
fprintf(outFile, '%u, ', times);
fprintf(outFile, '};\n');

fprintf(outFile, 'int gyros[] = {');
fprintf(outFile, '%d, ', gyros);
fprintf(outFile, '};\n');

fprintf(outFile, 'int accels[] = {');
fprintf(outFile, '%d, ', accels);
fprintf(outFile, '};\n');

fprintf(outFile, 'int mags[] = {');
fprintf(outFile, '%d, ', mags);
fprintf(outFile, '};\n');

fprintf(outFile, 'int alts[] = {');
fprintf(outFile, '%f, ', alts);
fprintf(outFile, '};\n');

fprintf(outFile, '#endinf');
fclose(outFile);
