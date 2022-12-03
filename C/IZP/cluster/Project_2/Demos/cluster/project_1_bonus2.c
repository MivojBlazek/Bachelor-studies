/**
 * Michal Blazek (xblaze38)
 * 30. listopad 2022
 * 
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D
    * Use without third parameter for single linkage method
    * Use parameter [-c] for complete linkage method
    * Use parameter [-k] for k-means method
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    //
    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(cap * sizeof(struct obj_t));
    //
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    //
    free(c->obj);
    init_cluster(c, 0);
    //
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    //
    //pokud neni misto pro objekt, rozsizi cluster
    if (c->size >= c->capacity)
    {
        if (resize_cluster(c, c->capacity + CLUSTER_CHUNK) == NULL)
        {
            fprintf(stderr, "ERROR with realloc()\n");
            return;
        }
    }
    //pokud je misto pro objekt, vlozi ho nakonec
    if (c->size < c->capacity)
    {
        c->obj[c->size] = obj;
        c->size++;
    }
    //
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    //
    for (int i = 0; i < c2->size; i++)
    {
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
    //
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    //
    //vycistim cluster na indexu idx
    clear_cluster(&carr[idx]);
    free(carr[idx].obj);
    //posunu clustery o jedno misto tak, aby v poli clusteru nebyla mezera
    for (int i = idx; i < narr - 1; i++)
    {
        carr[i] = carr[i + 1];
    }
    return --narr;
    //
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    //
    return sqrtf(powf(o1->x - o2->x, 2.0) + powf(o1->y - o2->y, 2.0));
    //
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2, int c_mode)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    //
    //vyberu si vzdalenost prvnich dvou
    float distance = obj_distance(&c1->obj[0], &c2->obj[0]);
    for (int i = 0; i < c1->size; i++)
    {
        for (int j = 0; j < c2->size; j++)
        {
            //porovnavam kazdy s kazdym a hledam podle parametru c_mode nejkratsi nebo nejvetsi vzdalenost
            if (c_mode == 0 && (obj_distance(&c1->obj[i], &c2->obj[j]) < distance))
                distance = obj_distance(&c1->obj[i], &c2->obj[j]);
            else if (c_mode == 1 && (obj_distance(&c1->obj[i], &c2->obj[j]) > distance))
                distance = obj_distance(&c1->obj[i], &c2->obj[j]);
        }
    }
    return distance;
    //
}

/*
 Pocita vzdalenost dvou shluku pro algoritmus k-means.
*/
float k_cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    struct obj_t obj1, obj2;
    float x = 0;
    float y = 0;
    
    //vypocte stred prvniho clusteru
    for (int i = 0; i < c1->size; i++)
    {
        x += c1->obj[i].x;
        y += c1->obj[i].y;
    }
    obj1.x = x / c1->size;
    obj1.y = y / c1->size;

    x = 0;
    y = 0;

    //vypocte stred druheho clusteru
    for (int i = 0; i < c2->size; i++)
    {
        x += c2->obj[i].x;
        y += c2->obj[i].y;
    }
    obj2.x = x / c2->size;
    obj2.y = y / c2->size;
    
    return obj_distance(&obj1, &obj2);
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2, int c_mode, int k_mode)
{
    assert(narr > 0);

    // TODO
    //
    *c1 = 0;
    *c2 = 1;
    float lowest_distance;

    //vyberu prvni dva jako nejkratsi
    if (k_mode)
        lowest_distance = k_cluster_distance(&carr[*c1], &carr[*c2]);
    else
        lowest_distance = cluster_distance(&carr[*c1], &carr[*c2], c_mode);
    for (int i = 0; i < narr; i++)
    {
        for (int j = i + 1; j < narr; j++)
        {
            //porovnavam kazdy s kazdym a hledam co nejkratsi
            if (k_mode && (k_cluster_distance(&carr[i], &carr[j]) < lowest_distance))
            {
                lowest_distance = k_cluster_distance(&carr[i], &carr[j]);
                *c1 = i;
                *c2 = j;
            }
            else if (!k_mode && (cluster_distance(&carr[i], &carr[j], c_mode) < lowest_distance))
            {
                lowest_distance = cluster_distance(&carr[i], &carr[j], c_mode);
                *c1 = i;
                *c2 = j;
            }
        }
    }
    //
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Kontroluje, jestli je spravne zadan vstupni soubor mimo prvni radek.
*/
int file_error_check(char *token, int line_cnt, int array[line_cnt][3], int i, int j)
{
    //kontrola vynechaneho cisla v souboru
    if (!token || token == NULL)
        return 1;

    if (j == 0)
    {
        //kontrola prekroceni rozsahu ID
        if (atol(token) > INT_MAX || atol(token) < 0)
            return 1;
        
        // kontrola jedinečnosti ID
        for (int k = 1; i - k >= 0; k++)
        {
            if (array[i - k][0] == atol(token))
                return 1;
        }
    }
    //kontrola prekroceni rozsahu souradnic
    else if (atol(token) > 1000 || atol(token) < 0)
        return 1;

    return 0;
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    //
    FILE *input;
    char first_line[] = "count=";
    char line[30];

    int file_error = 0;

    input = fopen(filename, "r");
    if (input == NULL)
    {
        *arr = NULL;
        return -2;
    }
    int sizeF = sizeof(first_line) / sizeof(char); //velikost 'count='
    int size = sizeof(line) / sizeof(char);

    //cteni a kontrola "count=" na 1. radku
    fgets(line, sizeF, input);
    for (int i = 0; i < sizeF; i++)
    {
        if (first_line[i] != line[i])
            file_error = 1;
    }

    //cteni a kontrola cisla za "count="
    fgets(line, size, input);
    int line_cnt = atol(line);
    if (line_cnt <= 0)
    {
        *arr = NULL;
        fclose(input);
        return line_cnt;
    }

    //ulozeni prvku v souboru do dvourozmerneho pole
    int array[line_cnt][3];
    for (int i = 0; i < line_cnt; i++)
    {
        if (fgets(line, size, input) == NULL)
        {
            file_error = 1;
            break;
        }

        //strtok rozdeluje radek podle mezer
        char *token = strtok(line, " ");
        for (int j = 0; j < 3; j++)
        {
            file_error = file_error_check(token, line_cnt, array, i, j);

            array[i][j] = atol(token);
            token = strtok(NULL, " ");
        }
    }
    
    //pokud nastala chyba tykajici se souboru
    if (file_error)
    {
        *arr = NULL;
        fclose(input);
        return -1;
    }

    //ulozeni objektu do pole clusteru
    struct obj_t o_arr[line_cnt];
    struct cluster_t *c_arr;
    c_arr = malloc(line_cnt * sizeof(struct cluster_t));

    for (int i = 0; i < line_cnt; i++)
    {
        o_arr[i].id = array[i][0];
        o_arr[i].x = array[i][1];
        o_arr[i].y = array[i][2];
        init_cluster(&c_arr[i], 1);
        append_cluster(&c_arr[i], o_arr[i]);
    }
    //odeslani na arr
    *arr = c_arr;

    fclose(input);
    return line_cnt;
    //
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

/*
 Kontroluje argumenty programu.
*/
int incompatible_args_check(int argc, char *argv[], int *c_mode, int *k_mode)
{
    if (argc != 2)
    {
        //kontrola parametru -c a -k na 4. pozici
        if (argc == 4)
        {
            if (argv[3][0] == '-' && argv[3][1] == 'c' && argv[3][2] == '\0')
                *c_mode = 1;
            if (argv[3][0] == '-' && argv[3][1] == 'k' && argv[3][2] == '\0')
                *k_mode = 1;
            return atol(argv[2]);
        }
        else if (argc == 3)
        {
            //kontrola parametru -c a -k na 3. pozici
            if (argv[2][0] == '-' && argv[2][1] == 'c' && argv[2][2] == '\0')
                *c_mode = 1;
            if (argv[2][0] == '-' && argv[2][1] == 'k' && argv[2][2] == '\0')
                *k_mode = 1;
            else
            {
                return atol(argv[2]);
            }
        }
        else
        {
            fprintf(stderr, "Incompatible arguments!\nUsage: %s 'filename' [number_of_clusters] [-c or -k]\n", argv[0]);
            return -1;
        }
    }
    //uzivatel zadal jen nazev souboru
    return 1;
}

/*
 Funkce pro uvolnovani alokovane pameti.
*/
void mem_free(struct cluster_t *carr, int narr)
{
    while (narr > 0)
    {
        narr = remove_cluster(carr, narr, narr - 1);
    }
    free(carr);
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    // TODO
    //
    int c_mode = 0;
    int k_mode = 0;
    
    //kontrola argumentu
    int final_narr = incompatible_args_check(argc, argv, &c_mode, &k_mode);
    if (final_narr == -1)
        return 1;

    int narr = load_clusters(argv[1], &clusters);

    //problem s daty v souboru
    if (narr == -1)
    {
        fprintf(stderr, "Data format in %s is incorrect!\nUsage:\ncount=NUMBER_OF_OBJECTS\nID X Y\nID X Y\n\nNUMBER_OF_OBJECTS is natural number under %d\nID is unique natural number under %d\nX and Y are integers from 0 to 1000\n", argv[1], INT_MAX, INT_MAX);
        free(clusters);
        return 1;
    }
    //problem s otevrenim souboru
    if (narr == -2)
    {
        fprintf(stderr, "Cannot open file '%s'!\nUsage: %s 'filename' [number_of_clusters] [-c or -k]\n", argv[1], argv[0]);
        return 1;
    }

    //kontorla parametru N
    if (final_narr > narr || final_narr <= 0)
    {
        fprintf(stderr, "Cannot make %d clusters with %d objects in %s!\n", final_narr, narr, argv[1]);
        mem_free(clusters, narr);
        return 1;
    }

    //algoritmus shlukovani
    //'najdi podobny' -> 'spoj je do prvniho' -> 'druhy smaz'
    int c1, c2;
    while (narr > final_narr)
    {
        find_neighbours(clusters, narr, &c1, &c2, c_mode, k_mode);
        merge_clusters(&clusters[c1], &clusters[c2]);
        narr = remove_cluster(clusters, narr, c2);
    }
    print_clusters(clusters, narr);

    //uvolneni zbylych clusteru
    mem_free(clusters, narr);

    return 0;
    //
}