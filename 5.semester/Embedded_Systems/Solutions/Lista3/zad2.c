#include<stdio.h>

void compress(FILE* input, FILE* output)
{
    char previous;
    char character;
    char count = '0';
    fread(&character, sizeof(char), 1, input);
    previous = character;
    do
    {
        if(previous == character && count < 57) //count <= '9'
        {
            count++;
            previous = character;
        }
        else
        {
            fwrite(&previous, sizeof(char), 1, output);
            fwrite(&count, sizeof(char), 1, output);
            count = '1';
            previous = character;
        }
    }
    while(fread(&character, sizeof(char), 1, input) == 1);
    fwrite(&previous, sizeof(char), 1, output);
    fwrite(&count, sizeof(char), 1, output);
}

void decompress(FILE* input, FILE* output)
{
    char character;
    char count;
    int int_count;
    int i;
    while(fread(&character, sizeof(char), 1, input) == 1)
    {
        fread(&count, sizeof(char), 1, input);
        int_count = count - '0';
        for(i=0; i<int_count; i++)
        {
            fwrite(&character, sizeof(char), 1, output);
        }
    }
}

int main(int argc, char** argv)
{
    if(argc != 4)
        return 1;
    char* inputPath = argv[1];
    char* outputPath = argv[2];
    int mode = atoi(argv[3]);
    FILE *in, *out;
    in = fopen(inputPath, "rb");
    out = fopen(outputPath, "wb");
    if(mode == 0)
    {
        compress(in, out);
    }
    else
    {
        decompress(in, out);
    }
    return 0;
}
