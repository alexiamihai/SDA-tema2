TEMA 2- Compresia imaginilor utilizand arbori

    Am avut nevoie de 4 structuri, una pentru a reprezenta matricea de pixeli si
valorile culorilor fiecarui nod, una pentru arborele cuaternar, in care am
retinut culorile, dimensiunea, tipul si copiii fiecarui nod si doua pentru
coada. Pentru rezolvarea cerintei 3, a fost necesara folosirea unei cozi si a
unor functii necesare lucrului cu aceasta, precum adauga_in_coada() si
scoate_din_coada().

CERINTA 1:

    1. calculeaza_mean():
        - functie care calculeaza culoarea medie a unui bloc din matricea de
        pixeli, pentru a determina scorul similaritatii si a stabili daca este
        necesara divizarea blocului.
        - am utilizat formulele date pentru a calcula scorul
        - functia primeste ca parametri matricea de pixeli, indicii coltului din
        stanga sus al unui bloc, dimensiunea acestuia, pragul cu care trebuie
        comparat scorul, un pointer catre variabila ok, folsita pentru a stabili
        daca scorul este, sau nu, mai mic decat pragul si trei pointeri catre
        variabilele r, g si b, in care vor fi retinute culorile medii, in cazul
        in care zona nu trebuie divizata

    2. divide():
        - functie care imparte imaginea in patru zone si atribuie fiecarei zone
        un scor de similaritate, pentru a decide daca aceasta trebuie sau nu
        divizata. Daca zona trebuie divizata, functia este reapeleaza recursiv,
        pentru zona respectiva. Fiecare nod pastreaza dimensiunea zonei pe care
        o reprezinta, iar in variabila nr, transmisa ca parametru, se numara
        nodurile cu scorul similaritatii mai mic decat pragul impus.

    3. niveluri():
        - functie care returneaza numarul de niveluri ale unui arbore
        - pentru a determina numarul de niveluri, se parcurg toti copiii nodului
        curent si se apeleaza recursiv functia niveluri pentru fiecare copil.
        Valoarea obtinuta se compara cu maximul actual, care este actualizat
        daca este cazul.

    4. dimensiune():
        - functie care returneaza dimensiunea laturii patratului pentru cea mai
        mare zona din imagine care a ramas nedivizata.
        - pentru a determina dimensiunea maxima, se parcurg toti copiii nodului
        curent si se verifica daca nodul respectiv nu este o frunza. Daca nodul
        nu este o frunza, functia se apeleaza recursiv, pana la intalnirea unei
        frunze. Altfel, dimensiunea curenta devine valoarea stocata in nod. Se
        compara dimensiunea gasita cu maximul actual si se actualizeaza valoarea
        maxima daca este cazul. La final, se returneaza dimensiunea maxima
        gasita.

CERINTA 2:

    1. parcurge_un_nivel():
        - functie care parcurge un nivel, trimis ca parametru, din arbore si
        scrie datele nodurilor de pe acel nivel in fisier
        - se trece la copiii nodului curent, scazandu-se nivelul initial, pana
        ce se ajunge la 0, caz in care s-a ajuns la nivelul dorit, iar datele
        nodului gasit acolo pot fi trecute in fisier
    2. parcurgere_pe_niveluri():
        - functie care parcurge fiecare nivel al arborelui, folosindu-se de
        functiile parcurge_un_nivel(), pentru a scrie in fisier si de niveluri()
        pentru a determina numarul de niveluri care trebuie parcurse.

CERINTA 3:

    1. citeste():
        - functie care citeste date dintr-un fisier binar si creeaza un arbore
        cuaternar
        - am citit separat valorile pentru primul nod al arborelui, pentru
        radacina, urmand ca prin intermediul unei structuri repetitive sa citesc
        restul copiilor. Am citit din fisier cat timp a fost posibila citirea
        tipului unui nod. Pentru frunze, am citit si valorile culorilor acestora
        - am tinut evidenta nodurilor care trebuie citite din fisier, per nivel,
        dar si a numarului de copii pentru fiecare nod in parte
        - cu ajutorul unei cozi, am tinut evidenta nodurilor care trebuie sa
        aiba copii. Nodurile de tip 0 sunt adaugate in coada, atunci cand sunt
        intalnite si scoase, o data ce sunt completate cu noduri copii.
    2. adauga_in_arbore():
        - functie care adauga un copil in arbore, pentru nodul trimis ca
        parametru
    3. creeaza_matrice():
        - functie care, plecand de la arborele construit anterior, creeaza o
        matrice de pixeli, folosita pentru decodificarea imaginii
        - matricea este divizata, pana ce se gaseste un copil de tip frunza care
        poate stabili culoarea corespunzatoare acelei zone din matrice
    4. afiseaza():
        - functie care afiseaza matricea de pixeli construita

    In main, m-am folosit de numarul de argumente pentru a determina tipul
cerintei si in functie de acesta, am scris in fisiere datele cerute si am apelat
functiile necesare.