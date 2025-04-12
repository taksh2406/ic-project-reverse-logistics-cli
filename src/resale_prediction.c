#include "resale_prediction.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define K 3              
#define MAX_ITER 100     
#define TOLERANCE 0.001f 


static float absDiff(float a, float b)
{
    return (a > b) ? (a - b) : (b - a);
}

static float computeMean(float data[], int count)
{
    if(count == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < count; i++)
    {
        sum += data[i];
    }
    return sum / count;
}

static void kMeansClustering(float data[], int n, float centroids[K])
{
    srand((unsigned int) time(NULL));
    for (int i = 0; i < K; i++)
    {
        int index = rand() % n;
        centroids[i] = data[index];
    }
    
    int assignments[n];

    for (int iter = 0; iter < MAX_ITER; iter++)
    {
        for (int i = 0; i < n; i++)
        {
            float minDist = INFINITY;
            int bestCluster = 0;
            for (int j = 0; j < K; j++)
            {
                float dist = fabs(data[i] - centroids[j]);
                if (dist < minDist)
                {
                    minDist = dist;
                    bestCluster = j;
                }
            }
            assignments[i] = bestCluster;
        }

        float newCentroids[K] = {0.0f};
        int counts[K] = {0};
        for (int i = 0; i < n; i++)
        {
            int cluster = assignments[i];
            newCentroids[cluster] += data[i];
            counts[cluster]++;
        }
        
        int converged = 1;
        for (int j = 0; j < K; j++)
        {
            if (counts[j] > 0)
            {
                newCentroids[j] /= counts[j];
            }
            if (fabs(newCentroids[j] - centroids[j]) > TOLERANCE)
            {
                converged = 0;
            }
            centroids[j] = newCentroids[j];
        }
        if (converged) break;
    }
}

static float predictResaleFactorFromCentroids(float conditionScore, float centroids[K])
{
    float minDiff = INFINITY;
    int bestIndex = 0;
    for (int j = 0; j < K; j++)
    {
        float diff = fabs(conditionScore - centroids[j]);
        if (diff < minDiff)
        {
            minDiff = diff;
            bestIndex = j;
        }
    }
    return centroids[bestIndex];
}

float predictResalePriceKMeans(float originalPrice, float conditionScore)
{
    // Historical resale factors (resalePrice/originalPrice ratios) from past products.
    float historicalData[] ={
        0.95f, 0.92f, 0.90f, 0.88f,  // High quality / New products
        0.80f, 0.78f, 0.75f, 0.77f,  // Refurbished products
        0.60f, 0.62f, 0.58f, 0.65f,  // Customer Remorse / Slightly used
        0.50f, 0.52f, 0.48f, 0.55f,  // Damaged products
        0.70f, 0.68f, 0.72f, 0.69f   // Average quality products
    };
    int n = sizeof(historicalData) / sizeof(historicalData[0]);
    
    float centroids[K];
    kMeansClustering(historicalData, n, centroids);
    

    // printf("Cluster centroids: ");
    // for (int j = 0; j < K; j++) { printf("%.2f ", centroids[j]); }
    // printf("\n");

    float predictedFactor = predictResaleFactorFromCentroids(conditionScore, centroids);

    float predictedResalePrice = originalPrice * predictedFactor;
    return predictedResalePrice;
}

