#include "webQuery.h"

void webQuery(struct listNode *urlHead)
{
    char userInput[MAX_USER_INPUT];
    char *queryTerms[MAX_USER_INPUT];
    int isInputValid;
    int i, numOfTerms;

    while (1)
    { // iterate until user enters empty input (i.e. just presses enter)
        do
        {
            printf("\n");
            getUserInput(userInput);
            numOfTerms = tokenizeUserInput(userInput, queryTerms);

            if (isUserInputEmpty(userInput))
                return; // return control back to main for clean up and exit

            isInputValid = isUserInputValid(queryTerms, numOfTerms);

            if (!isInputValid)
            {
                fflush(stdout);
                fprintf(stderr, "Please enter a query containing only lower-case letters.\n");
            }

        } while (!isInputValid);

        // print query information to stdout
        printQueryTerms(queryTerms, numOfTerms);

        printf("IDF scores are\n");
        int documentCount = getAddrCount(urlHead, 0);
        int atLeastOneOccurenceInDocumentArr[numOfTerms];

        for (i = 0; i < numOfTerms; i++)
        {
            int atLeastOneOccurence = getAtLeastOneDocumentOccurrence(urlHead, queryTerms[i], 0);
            atLeastOneOccurenceInDocumentArr[i] = atLeastOneOccurence;
            printf("IDF(%s): %.7f\n", queryTerms[i], calcInverseDocumentFrequency(documentCount, atLeastOneOccurence));
        }

        printf("Web pages:\n");
        calcScore(urlHead, queryTerms, numOfTerms, atLeastOneOccurenceInDocumentArr, documentCount);
        sortList(urlHead);
        printAddressesWithScores(urlHead, 1);
    }
}

void getUserInput(char *userInput)
{
    printf("Enter a web query: ");
    if (!fgets(userInput, MAX_USER_INPUT, stdin))
        userInput[0] = '\0';

    userInput[strlen(userInput) - 1] = '\0';
}

int isUserInputEmpty(char *userInput)
{
    return userInput[0] == '\0';
}

int tokenizeUserInput(char *userInput, char **queryTerms)
{
    char *delim = " \n";
    int numOfTerms = 0;
    int i = 0;
    char *token = strtok(userInput, delim);

    while (token != NULL)
    {
        queryTerms[i++] = token;
        token = strtok(NULL, delim);
        numOfTerms++;
    }

    return numOfTerms;
}

int isUserInputValid(char **queryTerms, int numOfTerms)
{
    int i, j;

    for (i = 0; i < numOfTerms; i++)
        for (j = 0; j < strlen(queryTerms[i]); j++)
            if (!islower(queryTerms[i][j]) || !isalpha(queryTerms[i][j]))
                return 0; // return false

    return 1; // return true
}

/* Print out all of the query terms. */
void printQueryTerms(char **queryTerms, int numOfTerms)
{
    int i;

    printf("Query is \"");
    for (i = 0; i < numOfTerms; i++)
    {
        if (i == numOfTerms - 1)
            printf("%s", queryTerms[i]); // last word should not have a space after it
        else
            printf("%s ", queryTerms[i]); // print word with a space after it
    }
    printf("\".\n");
}

/* Calculate the score for each web page and store it in the listNode. */
double calcTermFrequency(int termTotal, int documentTermTotal)
{
    return (double)termTotal / documentTermTotal;
}

/* Calculate the score for each web page and store it in the listNode. */
double calcInverseDocumentFrequency(int documentCount, int atLeastOneOccurence)
{
    return log((1.0 + documentCount) / (1.0 + atLeastOneOccurence));
}