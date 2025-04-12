#include <stdio.h>
#include <string.h>
#include "return_classification.h"

const char* classifyReturn(const char* condition, const char* returnReason)
{
    if (strcmp(condition, "Defective") == 0)
    {
        if (strcmp(returnReason, "Broken") == 0)
        {
            return "Refurbish";
        }
        else if (strcmp(returnReason, "Damaged") == 0)
        {
            return "Recycle";
        }else
        {
            return "Refurbish"; // Default for defective items
        }
    }
    else if (strcmp(condition, "Minor Issue") == 0)
    {
        return "Restock";
    }
    else if (strcmp(condition, "Customer Remorse") == 0)
    {
        return "Resell at Discount";
    }
    else
    {
        return "Manual Inspection Required";
    }
}

void processReturnClassification()
{
    char condition[50];
    char returnReason[50];

    printf("Enter product condition (e.g., Defective, Minor Issue, Customer Remorse): ");
    fgets(condition, sizeof(condition), stdin);
    condition[strcspn(condition, "\n")] = '\0';  

    printf("Enter return reason (e.g., Broken, Damaged): ");
    fgets(returnReason, sizeof(returnReason), stdin);
    returnReason[strcspn(returnReason, "\n")] = '\0';  

    const char* recommendation = classifyReturn(condition, returnReason);
    printf("Recommended Action: %s\n", recommendation);
}
