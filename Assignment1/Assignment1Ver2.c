#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------
// string lib
//------------------------------------------------------------------------
char *subStr(char *input, int start, int end);
int isAlpha(char ch);
int isNumber(char ch);
char *chInput();

//------------------------------------------------------------------------
// Term
//------------------------------------------------------------------------
typedef struct _Term
{
    int mExp;
    char mChTerm;
    struct _Term *mPtrTerm;
} Term;
Term *makeNewTerm();
void removeTerm(Term *term);
void mergeSort(Term *term, int start, int end);

//------------------------------------------------------------------------
// Monomial
//------------------------------------------------------------------------
typedef struct _Mono
{
    int mCeof, mSize;
    Term *mPtrTerm;
    struct _Mono *mPtrMono;
} Mono;
Mono *makeNewMono();
void removeMono(Mono *mono);
void addTermToMono(Mono *mono, Term *term);
void stringToMono(Mono *mono, char *str);

//------------------------------------------------------------------------
// Polymial
//------------------------------------------------------------------------
typedef struct _Poly
{
    int mSize, mConst;
    Mono *mPtrMono, *mPtrMonoBack;
} Poly;
Poly *makeNewPoly();
void removePoly(Poly *poly);
Poly *stringToPoly(char *str);
void printPoly(Poly *poly);

//------------------------------------------------------------------------
// Polymials
//------------------------------------------------------------------------
typedef struct _Polys
{
    Poly *mPtrPoly;
    struct _Polys *mPtrPolys;
} Polys;
Polys *makeNewPolys();
void removePolys(Polys *polys);
Polys *stringToPolys(char *str);
Poly *multPolys(Polys *polys);

//------------------------------------------------------------------------
// main
//------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    char *input = chInput();
    // printf("%s", input);
    // Poly *poly = stringToPoly(input);
    // printPoly(poly);
    Polys *polys = stringToPolys(input);
    Poly *poly = multPolys(polys);
    printPoly(poly);
    // printPoly(polys->mPtrPoly);

    return 0;
}

//------------------------------------------------------------------------
// string lib
//------------------------------------------------------------------------
char *subStr(char *str, int start, int end)
{
    char *res = malloc(sizeof(char) * (end - start + 1));
    strncpy(res, str + start, end - start);
    *(res + end - start) = 0;
    return res;
}
int isAlpha(char ch)
{
    return (ch >= 'a') && (ch <= 'z') || (ch >= 'A') && (ch <= 'Z');
}
int isNumber(char ch)
{
    return (ch >= '0') && (ch <= '9');
}
// input
char *chInput()
{
    char *input = NULL, *res = NULL;
    size_t sizeT = 0;
    int i, length = 0, size, cur = 0;

    // input
    printf("? ");
    getline(&input, &sizeT, stdin);
    size = strlen(input);

    // remove spacebar
    for (int i = 0; i < size; i++)
    {
        if (*(input + i) == ' ')
            continue;
        length++;
    }

    res = (char *)malloc(sizeof(char) * (length + 1));

    for (int i = 0; i < size; i++)
    {
        if (*(input + i) == ' ')
            continue;
        *(res + cur++) = *(input + i);
    }
    return res;
}
//------------------------------------------------------------------------
// Term
//------------------------------------------------------------------------
Term *makeNewTerm()
{
    Term *res = (Term *)malloc(sizeof(Term));
    res->mExp = 1;
    res->mPtrTerm = NULL;
}
void removeTerm(Term *term)
{
    if (term == NULL)
        return;
    free(term);
}

//------------------------------------------------------------------------
// Monomial
//------------------------------------------------------------------------
Mono *makeNewMono()
{
    Mono *res = (Mono *)malloc(sizeof(Mono));
    res->mCeof = 1;
    res->mSize = 0;
    res->mPtrTerm = NULL;
}
void removeMono(Mono *mono)
{
    if (mono->mPtrTerm)
        free(mono->mPtrTerm);
    free(mono);
}
void addTermToMono(Mono *mono, Term *term)
{
    if (mono->mPtrTerm)
    {
        mono->mPtrTerm = (Term *)realloc(mono->mPtrTerm, sizeof(Term) * (mono->mSize + 1));
        mono->mPtrTerm[mono->mSize] = *term;
    }
    else
    {
        mono->mPtrTerm = (Term *)malloc(sizeof(Term));
        *mono->mPtrTerm = *term;
    }
    mono->mSize++;
    removeTerm(term);
    return;
}

void stringToMono(Mono *mono, char *str)
{
    Term *term = NULL;
    int i, j, length = strlen(str);
    length -= (*(str + length - 1) == '\n');
    for (i = 0; i < length;)
    {
        term = makeNewTerm();
        term->mChTerm = *(str + i++);
        if (i < length && *(str + i) == '^')
        {
            i = j = i + 1;
            for (; j < length && isNumber(*(str + j)); j++)
                ;
            term->mExp = atoi(subStr(str, i, j));
            i = j;
        }
        addTermToMono(mono, term);
    }

    mergeSort(mono->mPtrTerm, 0, mono->mSize - 1);
    free(str);
}

// sort
void mergeSort(Term *term, int start, int end)
{
    if (start >= end)
        return;
    Term *ptrTmp = (Term *)malloc(sizeof(Term) * (end - start + 1));
    int mid = (start + end) / 2, l = start, r = mid + 1, idx = 0, i;
    mergeSort(term, start, mid);
    mergeSort(term, mid + 1, end);
    while (l <= mid || r <= end)
    {
        if (r > end || l <= mid && term[l].mChTerm < term[r].mChTerm)
        {
            ptrTmp[idx++] = term[l++];
        }
        else
        {
            ptrTmp[idx++] = term[r++];
        }
    }
    for (i = start; i <= end; i++)
    {
        term[i] = ptrTmp[i - start];
    }
}
Mono *multMono(Mono *a, Mono *b)
{
    int i = 0, j = 0;
    Mono *mono = makeNewMono();
    mono->mCeof = a->mCeof * b->mCeof;
    Term *term;
    while (i < a->mSize || j < b->mSize)
    {
        term = makeNewTerm();
        if (i < a->mSize && j < b->mSize && a->mPtrTerm[i].mChTerm == b->mPtrTerm[j].mChTerm)
        {
            term->mChTerm = a->mPtrTerm[i].mChTerm;
            term->mExp = a->mPtrTerm[i++].mExp + b->mPtrTerm[j++].mExp;
        }
        else if (j >= b->mSize || i < a->mSize && a->mPtrTerm[i].mChTerm < b->mPtrTerm[j].mChTerm)
        {
            *term = a->mPtrTerm[i++];
        }
        else
        {
            *term = b->mPtrTerm[j++];
        }
        addTermToMono(mono, term);
    }
    return mono;
}
int isMonoEqual(Mono *a, Mono *b)
{
    if (a->mSize != b->mSize)
        return 0;

    Term *ptri = a->mPtrTerm, *ptrj = b->mPtrTerm;
    while (ptri && ptrj)
    {
        if (ptri->mChTerm != ptrj->mChTerm || ptri->mExp != ptrj->mExp)
            return 0;
        ptri = ptri->mPtrTerm;
        ptrj = ptrj->mPtrTerm;
    }
    return 1;
}
//------------------------------------------------------------------------
// Polymial
//------------------------------------------------------------------------
Poly *makeNewPoly()
{
    Poly *poly = (Poly *)malloc(sizeof(Poly));
    poly->mSize = 0;
    poly->mConst = 0;
    poly->mPtrMono = NULL;
    poly->mPtrMonoBack = NULL;
}
void removePoly(Poly *poly)
{
    if (poly == NULL)
        return;

    Mono *tmp;
    while (poly->mPtrMono)
    {
        tmp = poly->mPtrMono;
        poly->mPtrMono = poly->mPtrMono->mPtrMono;
        removeMono(tmp);
    }
    free(poly);
}
Poly *stringToPoly(char *str)
{
    Poly *poly = makeNewPoly();
    Mono *mono;
    Term *term;
    int i, j, length = strlen(str), sign;
    length -= (*(str + length - 1) == '\n');
    for (i = 0; i < length;)
    {
        mono = makeNewMono();
        sign = 1;
        if (*(str + i) == '-' || *(str + i) == '+')
        {
            sign = *(str + i++) == '-' ? -1 : 1;
        }

        // when string starts with a number ex) 7x^7y^5z^4, 3xyz^2
        if (isNumber(*(str + i)))
        {
            for (j = i + 1; j < length && isNumber(*(str + j)); j++)
                ;
            if (j < length && isAlpha(*(str + j)))
            {
                mono->mCeof = sign * atoi(subStr(str, i, j));
                i = j;
                for (; j < length && *(str + j) != '-' && *(str + j) != '+'; j++)
                    ;
                stringToMono(mono, subStr(str, i, j));
                i = j;
            }
            // constant ex) +7, -8
            else
            {
                poly->mConst += sign * atoi(subStr(str, i, j));
                i = j;
                continue;
            }
        }
        // when string start with a character
        else
        {
            for (; j < length && *(str + j) != '-' && *(str + j) != '+'; j++)
                ;
            stringToMono(mono, subStr(str, i, j));
            i = j;
        }
        if (poly->mPtrMono == NULL)
        {
            poly->mPtrMono = poly->mPtrMonoBack = mono;
        }
        else
        {
            poly->mPtrMonoBack->mPtrMono = mono;
            poly->mPtrMonoBack = mono;
        }
        poly->mSize++;
    }
    free(str);
    return poly;
}

void printPoly(Poly *poly)
{
    int cmp = 0, i;
    while (poly->mPtrMono)
    {
        Mono *mono = poly->mPtrMono;

        if (mono->mCeof > 0 && cmp)
            printf("+");
        if (mono->mCeof == -1)
            printf("-");
        cmp = 1;

        if (mono->mCeof != 1 && mono->mCeof != -1)
            printf("%d", mono->mCeof);

        for (i = 0; i < mono->mSize; i++)
        {
            printf("%c^%d", mono->mPtrTerm[i].mChTerm, mono->mPtrTerm[i].mExp);
        }
        poly->mPtrMono = poly->mPtrMono->mPtrMono;
    }
    if (poly->mConst > 0)
        printf("+");
    if (poly->mConst)
        printf("%d", poly->mConst);
}

Poly *multPoly(Poly *a, Poly *b)
{
    Mono *ptri, *ptrj, *ptrTmp;
    Poly *poly = makeNewPoly();
    poly->mConst = a->mConst * b->mConst;
    for (ptri = a->mPtrMono; ptri; ptri = ptri->mPtrMono)
    {
        for (ptrj = b->mPtrMono; ptrj; ptrj = ptrj->mPtrMono)
        {
            ptrTmp = multMono(ptri, ptrj);
            if (poly->mPtrMono)
            {
                poly->mPtrMonoBack->mPtrMono = ptrTmp;
                poly->mPtrMonoBack = poly->mPtrMonoBack->mPtrMono;
            }
            else
            {
                poly->mPtrMono = poly->mPtrMonoBack = ptrTmp;
            }
            poly->mSize++;
        }
    }
    if (b->mConst)
    {
        for (ptri = a->mPtrMono; ptri; ptri = ptri->mPtrMono)
        {
            ptrTmp = makeNewMono();
            ptrTmp->mCeof = ptri->mCeof * b->mConst;
            ptrTmp->mSize = ptri->mSize;
            ptrTmp->mPtrTerm = (Term *)malloc(sizeof(Term) * ptri->mSize);
            memcpy(ptrTmp->mPtrTerm, ptri->mPtrTerm, sizeof(Term) * ptri->mSize);
            if (poly->mPtrMono)
            {
                poly->mPtrMonoBack->mPtrMono = ptrTmp;
                poly->mPtrMonoBack = poly->mPtrMonoBack->mPtrMono;
            }
            else
            {
                poly->mPtrMono = poly->mPtrMonoBack = ptrTmp;
            }
            poly->mSize++;
        }
    }
    if (a->mConst)
    {
        for (ptri = b->mPtrMono; ptri; ptri = ptri->mPtrMono)
        {
            ptrTmp = makeNewMono();
            ptrTmp->mCeof = ptri->mCeof * a->mConst;
            ptrTmp->mSize = ptri->mSize;
            ptrTmp->mPtrTerm = (Term *)malloc(sizeof(Term) * ptri->mSize);
            memcpy(ptrTmp->mPtrTerm, ptri->mPtrTerm, sizeof(Term) * ptri->mSize);
            if (poly->mPtrMono)
            {
                poly->mPtrMonoBack->mPtrMono = ptrTmp;
                poly->mPtrMonoBack = poly->mPtrMonoBack->mPtrMono;
            }
            else
            {
                poly->mPtrMono = poly->mPtrMonoBack = ptrTmp;
            }
            poly->mSize++;
        }
    }
    for (ptri = poly->mPtrMono; ptri; ptri = ptri->mPtrMono)
    {
        if (!ptri->mPtrMono)
            break;
        ptrTmp = ptri;
        for (ptrj = ptri->mPtrMono; ptrj; ptrj = ptrj->mPtrMono)
        {
            if (isMonoEqual(ptri, ptrj))
            {
                ptri->mCeof += ptrj->mCeof;
                ptrTmp->mPtrMono = ptrj->mPtrMono;
            }
            else
            {
                ptrTmp = ptrj;
            }
        }
    }
    return poly;
}

//------------------------------------------------------------------------
// Polymials
//------------------------------------------------------------------------
Polys *makeNewPolys()
{
    Polys *polys = (Polys *)malloc(sizeof(Polys));
    polys->mPtrPoly = NULL;
    polys->mPtrPolys = NULL;
}
void removePolys(Polys *polys)
{
    Polys *ptrTmp;
    while (polys)
    {
        ptrTmp = polys;
        removePoly(polys->mPtrPoly);
        polys = polys->mPtrPolys;
        free(ptrTmp);
    }
}
Polys *stringToPolys(char *str)
{
    Polys *polys = NULL, *res = NULL, *polysTmp = NULL;
    Poly *poly;
    int i, j, length = strlen(str), cnt;
    length -= (*(str + length - 1) == '\n');
    for (i = 0; i < length; i++)
    {
        if (*(str + i) == '{')
        {
            cnt = 1;
            for (j = i + 1; j < length; j++)
            {
                if (*(str + j) == '{')
                    cnt++;
                else if (*(str + j) == '}')
                    cnt--;
                if (cnt == 0)
                {
                    polysTmp = stringToPolys(subStr(str, i + 1, j));
                    break;
                }
            }
        }
        else if (*(str + i) == '(')
        {
            cnt = 1;
            for (j = i + 1;; j++)
            {
                if (*(str + j) == '(')
                    cnt++;
                else if (*(str + j) == ')')
                    cnt--;
                if (cnt == 0)
                {
                    polysTmp = stringToPolys(subStr(str, i + 1, j));
                    break;
                }
            }
        }
        else if (*(str + i) != '*')
        {
            for (j = i + 1; j < length; j++)
            {
                if (*(str + j) == '*' || *(str + j) == '{' || *(str + j) == '(')
                    break;
            }
            polysTmp = makeNewPolys();
            polysTmp->mPtrPoly = stringToPoly(subStr(str, i, j));
        }
        else
        {
            continue;
        }
        i = j;
        if (polys)
        {
            polys->mPtrPolys = polysTmp;
            polys = polys->mPtrPolys;
        }
        else
        {
            polys = res = polysTmp;
        }
    }
    free(str);
    return res;
}
Poly *multPolys(Polys *polys)
{
    if (!polys)
        return NULL;
    Polys *polysTmp = polys;
    Poly *poly = polys->mPtrPoly;
    polys = polys->mPtrPolys;
    for (; polys; polys = polys->mPtrPolys)
    {
        poly = multPoly(poly, polys->mPtrPoly);
    }
    removePolys(polysTmp);
    return poly;
}
