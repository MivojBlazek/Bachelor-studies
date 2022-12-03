/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
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
    //fprintf(stderr, "Init object: %p (malloc size: %ld)\n", c->obj, cap * sizeof(struct obj_t));
    //
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    //
    c->size = 0; //not sure, if tam musi byt
    c->capacity = 0; //not sure, if tam musi byt
    c->obj = NULL;
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
    if (c->size >= c->capacity)
    {
        if (resize_cluster(c, c->capacity ? c->capacity + CLUSTER_CHUNK : CLUSTER_CHUNK) == NULL)
        {
            fprintf(stderr, "ERROR with realloc()\n");
            return;
        }
    }
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
    while (c2->size > c1->capacity - c1->size)
    {
        if (resize_cluster(c1, c1->capacity ? c1->capacity + CLUSTER_CHUNK : CLUSTER_CHUNK) == NULL)
        {
            fprintf(stderr, "ERROR with realloc()\n");
            return;
        }
    }
    if (c2->size <= c1->capacity - c1->size)
    {
        for (int i = 0; i < c2->size; i++)
        {
            c1->obj[c1->size] = c2->obj[i];
            c1->size++;
        }
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
    clear_cluster(&carr[idx]);
    for (int i = idx; i < narr - 1; i++)
    {
        carr[i].size = carr[i + 1].size;
        carr[i].capacity = carr[i + 1].capacity;
        carr[i].obj = carr[i + 1].obj;
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
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    //
    float lowest_distance = obj_distance(&c1->obj[0], &c2->obj[0]);
    for (int i = 0; i < c1->size; i++)
    {
        for (int j = 0; j < c2->size; j++)
        {
            if (obj_distance(&c1->obj[i], &c2->obj[j]) < lowest_distance)
                lowest_distance = obj_distance(&c1->obj[i], &c2->obj[j]);
        }
    }
    return lowest_distance;
    //
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    //
    // co kdyz bude narr = 1 ? pak to neni error, ale nemam co porovnavat a pocitat
    *c1 = 0;
    *c2 = 1;
    float lowest_distance = cluster_distance(&carr[*c1], &carr[*c2]);
    for (int i = 0; i < narr; i++)
    {
        for (int j = i + 1; j < narr; j++)
        {
            if (cluster_distance(&carr[i], &carr[j]) < lowest_distance)
            {
                lowest_distance = cluster_distance(&carr[i], &carr[j]);
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
    char line[400]; //mozna muzeme udelat dynamicky alokovane //WTF^2

    input = fopen(filename, "r");
    if (input == NULL)
    {
        return 0;
    }

    int sizeF = sizeof(first_line) / sizeof(char); //velikost 'count='
    int size = sizeof(line) / sizeof(char);

    fgets(line, sizeF, input);

    //kontrola, zda je tam opravdu 'count='
    for (int i = 0; i < sizeF; i++)
    {
        if (first_line[i] != line[i])
        {
            return 0;
        }
    }

    fgets(line, size, input);
    int line_cnt = atoi(line);

    //ulozeni do dvourozmerneho pole (lze asi rovnou ukladat do o_arr)
    int array[line_cnt][3];
    for (int i = 0; fgets(line, size, input) != NULL; i++)
    {
        char *token = strtok(line, " ");
        for (int j = 0; token; j++)
        {
            array[i][j] = atoi(token);
            token = strtok(NULL, " ");
        }
    }
    
    //ulozeni do pole clusteru
    struct cluster_t c_arr[line_cnt];
    struct obj_t o_arr[line_cnt];
    for (int i = 0; i < line_cnt; i++)
    {
        o_arr[i].id = array[i][0];
        o_arr[i].x = array[i][1];
        o_arr[i].y = array[i][2];

        init_cluster(&c_arr[i], 1);
        append_cluster(&c_arr[i], o_arr[i]);
        //fprintf(stderr, "Address of %d. cluster: %p   Object: %p\n", i, &c_arr[i], &c_arr[i].obj);
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

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    // TODO
    //
    /*if (argc != 3)
    {
        fprintf(stderr, "Incompatible arguments!\nUsage: %s 'filename' 'number_of_clusters'\n", argv[0]);
        return 1;
    }

    int narr = load_clusters(argv[1], &clusters);
    int final_narr = atoi(argv[2]);
    if (final_narr > narr)
    {
        fprintf(stderr, "Cannot make %d clusters with only %d objects in %s!\n", final_narr, narr, argv[1]);
        return 1;
    }*/

    int narr = load_clusters("objekty.txt", &clusters);
    /*for (int i = 0; i < narr; i++)
    {
        fprintf(stderr, "C%d: %p\n", i, &clusters[i].obj);
    }*/
    
    int final_narr = 3;


    int c1;
    int c2;
    while (narr > final_narr)
    {
        //fprintf(stderr, "Address of cluster: %p   Object: %p\n", clusters, clusters->obj);
        //print_clusters(clusters, narr);
        find_neighbours(clusters, narr, &c1, &c2);
        merge_clusters(&clusters[c1], &clusters[c2]);
        narr = remove_cluster(clusters, narr, c2);
    }
    print_clusters(clusters, narr);


    //uvolneni pameti
    /*for (int i = 0; i < narr; i++)
    {
        free(clusters[i].obj);
    }*/
    
    return 0;
    //
}