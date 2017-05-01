data = csvread('RawWithNoise1.csv', 1, 0);
times = data(:, 1);
gyros = data(:, 2);
accels = data(:, 3);
mags = data(:, 4);
alts = data(:, 5);
temps = data(:, 6);

length = size(times, 1);
%length = 500;

MPUFile = fopen('testMPU.bin', 'w');

for i = 1:length
   fwrite(MPUFile, times(i), 'int');
   fwrite(MPUFile, gyros(i), 'short');
   fwrite(MPUFile, accels(i), 'short');
   fwrite(MPUFile, mags(i), 'short');
end

fclose(MPUFile);

MPLFile = fopen('testMPL.bin', 'w');

for i = 1:length
   fwrite(MPLFile, times(i), 'int');
   fwrite(MPLFile, alts(i), 'float');
   fwrite(MPLFile, temps(i), 'float');
end

fclose(MPLFile);
