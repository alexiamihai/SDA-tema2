#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structura pentru pixel
typedef struct pixel
{
    unsigned char red, green, blue;
} pixel;

// structura pentru arborele cuaternar
typedef struct tree
{
    pixel *val;
    int dimensiune;
    unsigned char tip;
    struct tree *copii[4];
} tree;

/* coada implementata printr-o lista simplu inlantuita. am retinut
adresele primului si ultimului element din aceasta */
typedef struct element
{
    tree *nod;
    struct element *next;
} element;

typedef struct coada
{
    element *primul;
    element *ultimul;
} coada;

/* functie care calculeaza culoarea medie a unui bloc si apoi determina scorul
 similiaritatii pentru blocul respectiv, pentru a stabili daca este sau nu
 nevoie de divizare */
void calculeaza_mean(pixel **grid, int x, int y, int size, int factor, int *ok,
            unsigned long long *r, unsigned long long *g, unsigned long long *b)
{
    int i, j;
    unsigned long long red, green, blue, s1 = 0, s2 = 0, s3 = 0, mean, s4 = 0;
    // am folosit variabila ok pentru a retine daca valoarea scorului de
    // similiaritate este mai mica sau egala decat pragul impus
    *ok = 0;
    // am calculat sumele necesare pentru determinarea culorii medie
    for (i = x; i < x + size; i++)
    {
        for (j = y; j < y + size; j++)
        {
            s1 = s1 + (int)grid[i][j].red;
            s2 = s2 + (int)grid[i][j].green;
            s3 = s3 + (int)grid[i][j].blue;
        }
    }
    /* am calculat media aritmetica a valorilor din submatricea de pixeli,
    valori ce vor fi retinute in variabile pe care apoi le voi transmite ca
    parametri in functia divide, acestea reprezentand culorile nodurilor de
    tip frunza */
    red = s1 / (size * size);
    green = s2 / (size * size);
    blue = s3 / (size * size);
    *r = red;
    *g = green;
    *b = blue;
    // am calculat scorul similaritatii, folosindu-ma de formulele primite
    for (i = x; i < x + size; i++)
    {
        for (j = y; j < y + size; j++)
        {
            s4 = s4 + (red - (int)grid[i][j].red) * (red - (int)grid[i][j].red)
             + (green - (int)grid[i][j].green) * (green - (int)grid[i][j].green)
             + (blue - (int)grid[i][j].blue) * (blue - (int)grid[i][j].blue);
        }
    }
    mean = s4 / (3 * size * size);
    // daca scorul este mai mare, atunci ok devine 1
    if (mean > factor)
    {
        *ok = 1;
    }
}
// functie pentru impartirea imaginii in 4 zone
// i si j reprezinta indicii coltului din stanga sus al unei zone a matricei
void divide(tree *root, pixel **grid, int i, int j, int size, int factor,
            int *nr)
{
    unsigned long long r, g, b;
    int ok;
    // dimensiunea initiala a matricei se injumatateste, pentru ca fiecare noua
    // zona va avea dimensiunile egale cu jumatate din dimensiunile initiale
    size = size / 2;
    // am alocat memorie pentru copiii nodului initial si pentru structura in
    // care sunt retinute valorile pixelilor
    root->copii[0] = calloc(1, sizeof(tree));
    root->copii[0]->val = (pixel *)malloc(sizeof(pixel));
    root->copii[1] = calloc(1, sizeof(tree));
    root->copii[1]->val = (pixel *)malloc(sizeof(pixel));
    root->copii[2] = calloc(1, sizeof(tree));
    root->copii[2]->val = (pixel *)malloc(sizeof(pixel));
    root->copii[3] = calloc(1, sizeof(tree));
    root->copii[3]->val = (pixel *)malloc(sizeof(pixel));
    /* pentru fiecare zona a matricei, echivalenta unuia dintre cei 4 copii ai
    nodului, am calculat valoarea scorului de similiaritate
    prin intermediul variabilei ok, trimisa ca parametru, am decis daca
    zona trebuie, sau nu, divizata */
    calculeaza_mean(grid, i, j, size, factor, &ok, &r, &g, &b);
    /* daca ok este egal cu 1, atunci scorul de similaritate este mai mare
    decat pragul, caz in care copilul nodului este de tip 0 si se reapeleaza
    functia divide, deoarece zona trebuie impartita
    de asemenea, fiecare nod pastreaza in campul dimensiune, dimensiunea
    zonei pe care o reprezinta, iar in variabila nr, transmisa ca parametru,
    numar nodurile cu scorul similaritatii mai mic decat pragul impus */
    if (ok == 1)
    {
        root->copii[0]->tip = 0;
        root->copii[0]->dimensiune = size;
        // primul copil al nodului va avea mereu indicii ce indica pozitia din
        // coltul din stanga sus al zonei totale
        divide(root->copii[0], grid, i, j, size, factor, nr);
    }
    /* daca ok este 0, atunci nodul este o frunza, iar zona nu trebuie divizata,
    caz in care nodul pastreaza in campurile structurii sale valorile medii
    ale culorilor, calculate anterior, in cadrul functiei calculeaza_mean */
    else
    {
        (*nr)++;
        root->copii[0]->tip = 1;
        root->copii[0]->dimensiune = size;
        root->copii[0]->val->red = r;
        root->copii[0]->val->green = g;
        root->copii[0]->val->blue = b;
    }

    calculeaza_mean(grid, i, j + size, size, factor, &ok, &r, &g, &b);
    if (ok == 1)
    {
        root->copii[1]->tip = 0;
        root->copii[1]->dimensiune = size;
        /* al doilea copil al nodului va avea mereu indicii ce indica pozitia
        din coltul din stanga sus al sfertului din partea din dreapta sus a
        imaginii */
        divide(root->copii[1], grid, i, j + size, size, factor, nr);
    }
    else
    {
        (*nr)++;
        root->copii[1]->tip = 1;
        root->copii[1]->dimensiune = size;
        root->copii[1]->val->red = r;
        root->copii[1]->val->green = g;
        root->copii[1]->val->blue = b;
    }
    calculeaza_mean(grid, i + size, j + size, size, factor, &ok, &r, &g, &b);
    if (ok == 1)
    {
        root->copii[2]->tip = 0;
        root->copii[2]->dimensiune = size;
        /* al treilea copil al nodului va avea mereu indicii ce indica pozitia
        din coltul din stanga sus al sfertului din partea din dreapta jos a
        imaginii */
        divide(root->copii[2], grid, i + size, j + size, size, factor, nr);
    }
    else
    {
        (*nr)++;
        root->copii[2]->tip = 1;
        root->copii[2]->dimensiune = size;
        root->copii[2]->val->red = r;
        root->copii[2]->val->green = g;
        root->copii[2]->val->blue = b;
    }
    calculeaza_mean(grid, i + size, j, size, factor, &ok, &r, &g, &b);
    if (ok == 1)
    {
        root->copii[3]->tip = 0;
        root->copii[3]->dimensiune = size;
        /* al doilea copil al nodului va avea mereu indicii ce indica pozitia
        din coltul din stanga sus al sfertului din partea din stanga jos a
        imaginii */
        divide(root->copii[3], grid, i + size, j, size, factor, nr);
    }
    else
    {
        (*nr)++;
        root->copii[3]->tip = 1;
        root->copii[3]->dimensiune = size;
        root->copii[3]->val->red = r;
        root->copii[3]->val->green = g;
        root->copii[3]->val->blue = b;
    }
}
// functie pentru calculul numarului de niveluri al arborelui cuaternar
int niveluri(tree *root)
{
    // am verificat mai intai daca nodul este nul, caz in care inaltimea
    // arborelui nu este modificata
    if (root == NULL)
    {
        return 0;
    }
    int i, max = 0, nivel;
    /* pentru fiecare copil al nodului curent am apelat recursiv functia
    initiala pentru a determina inaltimea curenta, pe care am comparat-o
    apoi cu valoarea maxima actuala, retinuta de variabila max */
    for (i = 0; i < 4; i++)
    {
        nivel = niveluri(root->copii[i]);
        if (nivel > max)
        {
            max = nivel;
        }
    }
    // in final am returnat inaltimea maxima si am adaugat 1 pentru a include
    // si radacina in calcul
    return max + 1;
}
// functie pentru calculul dimensiunii laturii patratului pentru cea mai mare
// zona din imagine care a ramas nedivizata
int dimensiune(tree *root)
{
    if (root == NULL)
    {
        return 0;
    }
    int i, max = 0, dim;
    // pentru fiecare nod am verificat daca acesta nu este de tip frunza, caz in
    // care nu ar avea copii
    for (i = 0; i < 4; i++)
    {
        if (root->copii[i] != NULL)
        {
            // daca nodul nu este o frunza, am reapelat functia pentru acesta
            if (root->copii[i]->tip != 1)
            {
                dim = dimensiune(root->copii[i]);
            }
            // altfel, am retinut in variabila dim, dimensiunea zonei gasite,
            // stocata in nodul copil
            if (root->copii[i]->tip == 1)
            {
                dim = root->copii[i]->dimensiune;
            }
        }
        // am comparat dimensiunea gasita cu maximul curent
        if (dim > max)
        {
            max = dim;
        }
    }
    // la final, am returnat dimensiunea maxima
    return max;
}
// functie care parcurge un nivel al arborelui si scrie informatiile necesare
// in fisierul binar
void parcurge_un_nivel(tree *root, int nivel, FILE *f)
{
    int i;
    if (root == NULL)
    {
        return;
    }
    /* am primit ca parametru nivelul care trebuie parcurs
    in cazul in care variabila in care este retinut acesta ajunge la 0,
    inseamna ca s-a ajuns la nivelul dorit si informatiile nodului pot fi
    scrise in fisier
    in caz contrar, functia este reapelata pentru toti copiii nodului,
    reducand nivelul cu 1 */
    if (nivel == 0)
    {
        // pentru ambele feluri de noduri, tipul este scris in fisier
        // in plus, pentru frunze, am scris si valorile culorilor nodului
        fwrite(&root->tip, sizeof(unsigned char), 1, f);
        if (root->tip == 1)
        {
            fwrite(&root->val->red, sizeof(unsigned char), 1, f);
            fwrite(&root->val->green, sizeof(unsigned char), 1, f);
            fwrite(&root->val->blue, sizeof(unsigned char), 1, f);
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            parcurge_un_nivel(root->copii[i], nivel - 1, f);
        }
    }
}
void parcurgere_pe_niveluri(tree *root, FILE *f)
{
    // am folosit functia niveluri pentru a stii cate niveluri urmeaza sa fie
    // parcurse
    int inaltime = niveluri(root);
    int i;
    for (i = 0; i < inaltime; i++)
    {
        parcurge_un_nivel(root, i, f);
    }
}

// functie pentru adaugarea unui element in coada
void adauga_in_coada(coada *c, tree *nod)
{
    // aloca memorie pentru noul element adaugat
    element *nou = calloc(1, sizeof(element));
    nou->nod = nod;
    nou->next = NULL;
    // cazul in care coada este goala, iar pointerii primul si ultimul, care
    // indica elementele din capetele cozii, indica spre noul element
    if (c->primul == NULL)
    {
        c->primul = nou;
        c->ultimul = nou;
    }
    // altfel, acesta este adaugat la finalul cozii si ultimul indica spre el
    else
    {
        c->ultimul->next = nou;
        c->ultimul = nou;
    }
}
// functie pentru scoaterea din coada
void scoate_din_coada(coada *c)
{
    // cazul in care coada este goala
    if (c->primul == NULL)
    {
        return;
    }
    // folosesc pointerul aux pentru a elibera memoria alocata pentru
    // elementul ce trebuie eliminat
    element *aux;
    aux = c->primul;
    // se scoate primul element din coada
    c->primul = c->primul->next;
    free(aux);
}
// functie care primeste un nod si adauga un copil pentru acesta in pozitia
// specificata de variabila i
void adauga_in_arbore(tree *root, int i, int tip, int r, int g, int b, coada *c)
{
    // informatiile despre copil sunt transmise ca parametri
    int j;
    root->copii[i]->tip = tip;
    // in functie de tipul copilului, acesta se stabilesc valorile acestuia
    if (tip == 1)
    {
        root->copii[i]->val = malloc(sizeof(pixel));
        root->copii[i]->val->red = r;
        root->copii[i]->val->green = g;
        root->copii[i]->val->blue = b;
        root->copii[i]->dimensiune = root->dimensiune / 2;
        for (j = 0; j < 3; j++)
        {
            root->copii[i]->copii[j] = NULL;
        }
    }
    else
    {
        root->copii[i]->val = (pixel *)malloc(sizeof(pixel));
        root->copii[i]->val->blue = 0;
        root->copii[i]->val->green = 0;
        root->copii[i]->val->red = 0;
        // root->copii[i]->tip = 0;
        root->copii[i]->dimensiune = root->dimensiune / 2;
        /* daca nodul copil este de tip 0, atunci el este adaugat in coada,
        intrucat va fi necesar sa-i adaug si lui copii, dupa ce nodurile
        de dinaintea lui vor fi complete */
        adauga_in_coada(c, root->copii[i]);
    }
}
// functie care citeste valori din fisierul binar comprimat, folosindu-se de o
// coada care tine evidenta nodurilor care trebuie umplute cu date
void citeste(FILE *f, tree *root, coada *c, int *width)
{
    int size, i = 0, noduri = 4, nr = 0, contor = 1;
    // am folosit variabila nr pentru a calcula numarul de noduri ce ar trebui
    // sa se gaseasca pe nivelul urmator, valoare care mai apoi va fi
    // retinuta in variabila noduri
    // am folosit variabila contor pentru a numara cate noduri au fost adaugate
    // pe un nivel
    unsigned char tip, red, blue, green;
    // initial, am citit dimensiunea si tipul radacinei
    fread(&size, sizeof(int), 1, f);
    *width = size;
    fread(&tip, sizeof(unsigned char), 1, f);
    // daca radacina este frunza, atunci i-am citit si valorile culorilor
    if (tip == 1)
    {
        fread(&red, sizeof(unsigned char), 1, f);
        fread(&green, sizeof(unsigned char), 1, f);
        fread(&blue, sizeof(unsigned char), 1, f);
        root->tip = 1;
        root->val->red = red;
        root->val->green = green;
        root->val->blue = blue;
        root->dimensiune = size;
    }
    // daca nu, am adaugat-o in coada, intrucat inseamna ca va avea 4 noduri
    // copii
    else
    {
        root->tip = 0;
        root->dimensiune = size;
        adauga_in_coada(c, root);
    }
    // cat timp este posibila citirea tipului unui nod, am continuat sa citesc
    // din fisier
    while (fread(&tip, sizeof(unsigned char), 1, f) == 1)
    {
        // daca tipul este 1, atunci inseamna ca pot fi citite si valorile
        // culorilor pentru acel nod
        if (tip == 1)
        {
            fread(&red, sizeof(unsigned char), 1, f);
            fread(&green, sizeof(unsigned char), 1, f);
            fread(&blue, sizeof(unsigned char), 1, f);
        }
        else
        {
            red = -1;
            green = -1;
            blue = -1;
            nr = nr + 4;
        }
        /* variabila i retine numarul copilului care trebuie adaugat pentru
        primul nod din coada
        am alocat memorie pentru copilul nodului si apoi am folosit o
        functie pentru a-l adauga in arbore */
        c->primul->nod->copii[i] = calloc(1, sizeof(tree));
        adauga_in_arbore(c->primul->nod, i, tip, red, green, blue, c);
        /* in cazul in care al patrulea copil a fost adaugat in arbore pentru un
        anumit nod, acel nod este scos din coada, urmand sa fie completati
        copiii urmatorului nod */
        if (i % 3 == 0 && i != 0)
        {
            scoate_din_coada(c);
            i = -1;
        }
        // am comparat numarul de noduri adaugate pe nivel cu numarul total de
        // noduri de pe acel nivel
        if (contor == noduri)
        {
            // daca nivelul este complet, atunci noduri retine numarul de noduri
            // de pe nivelul urmator
            noduri = nr;
            contor = 0;
            nr = 0;
        }
        i++;
        contor++;
    }
}
// functie care creeaza matricea de pixeli, necesara pentru decodificarea
// imaginii comprimate
void creeaza_matrice(tree *root, pixel **grid, int x, int y)
{
    int i, j;
    /* daca nodul este frunza, atunci pixelii din zona din matrice, ce are ca
    indici din stanga sus valorile x si y, trimise ca parametri si ca dimensiune
    valoarea stocata in nod, vor avea culoarea specificata de nod */
    if (root->tip == 1)
    {
        for (i = x; i < x + root->dimensiune; i++)
        {
            for (j = y; j < y + root->dimensiune; j++)
            {
                grid[i][j].red = root->val->red;
                grid[i][j].green = root->val->green;
                grid[i][j].blue = root->val->blue;
            }
        }
    }
    /* altfel, matricea este divizata, iar functia este apelata recursiv pentru
    fiecare copil, cautandu-se pentru fiecare zona, o frunza care poate
    seta culorile pixelilor */
    else
    {
        creeaza_matrice(root->copii[0], grid, x, y);
        creeaza_matrice(root->copii[1], grid, x, y + root->dimensiune / 2);
        creeaza_matrice(root->copii[2], grid, x + root->dimensiune / 2,
                        y + root->dimensiune / 2);
        creeaza_matrice(root->copii[3], grid, x + root->dimensiune / 2, y);
    }
}
// functie care scrie in fisier matricea de pixeli construita
void afiseaza(FILE *f, pixel **grid, int size)
{
    int i, j;
    fprintf(f, "P6\n%d %d\n255\n", size, size);
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            fwrite(&grid[i][j].red, sizeof(unsigned char), 1, f);
            fwrite(&grid[i][j].green, sizeof(unsigned char), 1, f);
            fwrite(&grid[i][j].blue, sizeof(unsigned char), 1, f);
        }
    }
}
// functie pentru eliberarea memoriei alocate pentru arborele cuaternar
void free_tree(tree *root)
{
    if (root == NULL)
    {
        return;
    }
    free_tree(root->copii[0]);
    free_tree(root->copii[1]);
    free_tree(root->copii[2]);
    free_tree(root->copii[3]);
    free(root->val);
    free(root);
}

int main(int argc, char *argv[])
{
    int factor = 0, nr_blocuri = 0;
    char optiune[4];
    FILE *f1, *f2;
    /* daca numarul de argumente este egal cu 5, inseamna ca si factorul este
    unul dintre argumente, caz in care este clar ca se cere fie rezolvarea
    cerintei 1, fie rezolvarea cerintei 2
    am retinut optiunea in sirul optiune, si pragul in variabila factor,
    folosindu-ma de o functie pentru a converti string ul intr-un numar, iar
    apoi am deschis cele 2 fisiere, in modurile necesare */
    if (argc == 5)
    {
        strcpy(optiune, argv[1]);
        factor = atoi(argv[2]);
        f1 = fopen(argv[3], "rb");
        f2 = fopen(argv[4], "wb");
    }

    else
    {
        strcpy(optiune, argv[1]);
        f1 = fopen(argv[2], "rb");
        f2 = fopen(argv[3], "wb");
    }
    // am alocat memorie pentru radacina
    tree *root = calloc(1, sizeof(tree));
    root->val = (pixel *)malloc(sizeof(pixel));
    char tip[3];
    int width, height, valoare, i, j, compresie = 0;
    unsigned long long r, g, b;
    // cazul in care se cere rezolvarea cerintei 1 sau cerintei 2
    if (argc == 5)
    {
        // am citit datele necesare din fisier
        fscanf(f1, "%2s", tip);
        fseek(f1, 1, SEEK_CUR);
        fscanf(f1, "%d", &width);
        fseek(f1, 1, SEEK_CUR);
        fscanf(f1, "%d", &height);
        fseek(f1, 1, SEEK_CUR);
        fscanf(f1, "%d", &valoare);
        fseek(f1, 1, SEEK_CUR);
        // am alocat memorie pentru matricea de pixeli si apoi am citit datele
        // din fisier
        pixel **grid = malloc(height * sizeof(pixel *));
        for (i = 0; i < height; i++)
        {
            grid[i] = malloc(width * sizeof(pixel));
            for (j = 0; j < width; j++)
            {
                fread(&grid[i][j].red, sizeof(unsigned char), 1, f1);
                fread(&grid[i][j].green, sizeof(unsigned char), 1, f1);
                fread(&grid[i][j].blue, sizeof(unsigned char), 1, f1);
            }
        }
        // am tratat separat cazul in care imaginea initiala este cea despre
        // care trebuie sa determin daca va fi sau nu divizata
        calculeaza_mean(grid, 0, 0, width, factor, &compresie, &r, &g, &b);
        /* in cazul in care aceasta nu trebuie divizata, inseamna ca arborele va
        avea un singur nod de tip 1, dimensiunea maxima va fi dimensiunea
        radacinii, iar numarul de blocuri cu scorul similaritatii mai mic
        decat pragul va fi 1 */
        if (compresie == 0)
        {
            root->tip = 1;
            root->dimensiune = width;
            root->val->red = r;
            root->val->green = g;
            root->val->blue = b;
            nr_blocuri = 1;
        }
        // altfel se imparte imaginea in 4 zone, cu ajutorul functiei divide
        else
        {
            divide(root, grid, 0, 0, width, factor, &nr_blocuri);
        }
        // cazul in care se cere rezolvarea cerintei 1
        if (strcmp(optiune, "-c1") == 0)
        {
            /* daca radacina este frunza, atunci nu mai este nevoie sa se
            apeleze functiile create, intrucat informatiile cerute sunt
            deja cunoscute */
            if (root->tip == 1)
            {
                fprintf(f2, "1\n");
                fprintf(f2, "%d\n", nr_blocuri);
                fprintf(f2, "%d\n", root->dimensiune);
            }
            // altfel, am apelat functiile create
            else
            {
                fprintf(f2, "%d\n", niveluri(root));
                fprintf(f2, "%d\n", nr_blocuri);
                fprintf(f2, "%d\n", dimensiune(root));
            }
        }
        // cazul in care se cere rezolvarea cerintei 2
        if (strcmp(optiune, "-c2") == 0)
        {
            /* am scris mai intai dimensiunea initiala a matricei si apoi am
            utilizat o functie pentru a scrie date despre fiecare nod, in
            ordinea nivelurilor, in fisier */
            fwrite(&width, sizeof(unsigned int), 1, f2);
            parcurgere_pe_niveluri(root, f2);
        }
        // am eliberat memoria folosita pentru matricea de pixeli
        for (i = 0; i < height; i++)
        {
            free(grid[i]);
        }
        free(grid);
    }
    // cazul in care se cere rezolvarea cerintei 3
    else
    {
        // am alocat spatiu pentru coada
        coada *c = calloc(1, sizeof(coada));
        int size;
        // am citit din fisier valorile nodurilor si am creat arborele
        citeste(f1, root, c, &size);
        // am alocat spatiu pentru matricea de pixeli
        pixel **grid = malloc(size * sizeof(pixel *));
        for (i = 0; i < size; i++)
        {
            grid[i] = malloc(size * sizeof(pixel));
        }
        // folosindu-ma de arbore, am creat matricea de pixeli, iar apoi am
        // afisat-o in fisier
        creeaza_matrice(root, grid, 0, 0);
        afiseaza(f2, grid, size);
        // am eliberat memoria folosita pentru matricea de pixeli
        for (i = 0; i < size; i++)
        {
            free(grid[i]);
        }
        free(grid);
        free(c);
    }
    free_tree(root);
    fclose(f1);
    fclose(f2);
    return 0;
}
