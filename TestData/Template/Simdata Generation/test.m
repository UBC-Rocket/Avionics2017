inFile = fopen('test.bin', 'r');

for i = 1:5
    fread(inFile, [32], 'bit1')
    fread(inFile, 1, 'float');
end