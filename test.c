
#include <stdio.h>

float GenerateNoise(int x, int y, int seed)
{
    int n = x + y * 57 + seed * 131;
    n = (n << 13) ^ n;
    float noise = (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7FFFFFFF) / 1073741824.0f);

    // 将噪声值缩放到[0, 1000]范围并减缓过度
    int scaledNoise = (int)(noise * 1000+1000);

    return scaledNoise;
}


int main(void)
{
    const int mapSize = 9;
    const int seed = 0;

    for (int y = 0; y < mapSize; y++)
    {
        for (int x = 0; x < mapSize; x++)
        {
            float noise = GenerateNoise(x, y, seed);
            printf("Noise at (%d, %d): %f\n", x, y, noise);
        }
    }

    return 0;
}
