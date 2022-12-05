/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
/*
Matyáš Sapík
xsapik02
1.12.22
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <ctype.h>

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
    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(cap * sizeof(struct obj_t));
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO

    free(c->obj);
    c->size = 0;
    c->capacity = 0;
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
    while(c->capacity  <= c->size)
    {
    	if (resize_cluster(c, (c->capacity + 1)) == NULL)
        {
            fprintf(stderr, "ERROROAORRIAORKOAR\n");
            return;
        }
    }
    c->obj[c->size] = obj;
    c->size++;
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
    int size = c2->size;
    for(int i = 0; i < size; i++)
    {
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
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
    clear_cluster(&carr[idx]);
    for(int i = idx; i < narr - 1; i++)
    {
	    carr[i] = carr[i + 1];	
    }
    return narr - 1;
}
    

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    return sqrtf(pow((o1->x - o2->x), 2) + pow((o1->y - o2->y), 2));
}

/*
 `Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    float mind = sqrtf(pow(1000, 2) + pow(1000, 2));
    for(int x = 0; x < c1->size; x++)
    {
	    for(int y = 0; y < c2->size; y++)
	    {
	        if(mind > obj_distance(&c1->obj[x], &c2->obj[y]))
		        mind = obj_distance(&c1->obj[x], &c2->obj[y]);
	    }
    }
    return mind;
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
    float min = sqrtf(pow(1000, 2) + pow(1000, 2));
    for(int y = 0; y < narr; y++)
    {
	    for(int z = y + 1; z < narr; z++)
	    {
	        if(min > cluster_distance(&carr[y], &carr[z]))
	        {
		        min = cluster_distance(&carr[y], &carr[z]);
		        *c1 = y;
		        *c2 = z;
	        }
	    }
    }
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
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL)
	    return 0;
    char *line = "count=";
    char str[20];
    int size_str = sizeof(str) / sizeof(char);
    const int size_first_line = 6;
    fgets(str, size_first_line + 1, fp);
    for(int i = 0; i < size_first_line; i++)
    {
	    if(str[i] != line[i])
	        return -1;
    }
    fgets(str, size_str, fp);
    if(atoi(str) <= 0)
        return -2;
    int count_of_lines = atoi(str);
    struct cluster_t *array_c = malloc(count_of_lines * sizeof(struct cluster_t));
    for(int z = 0; z < count_of_lines; z++)
    {
    	init_cluster(&array_c[z], 1);
    }
    struct obj_t array_o[count_of_lines];
    int i = 0;
    while ((i < count_of_lines) && (fscanf(fp, "%d  %f  %f", &array_o[i].id, &array_o[i].x, &array_o[i].y) != EOF))
    {
	    if(array_o[i].x > 1000)
	        array_o[i].x = 1000;
	    else if(array_o[i].x < 0)
	        array_o[i].x = 0;

	    if(array_o[i].y > 1000)
	        array_o[i].y = 1000;
	    else if(array_o[i].y < 0)
	        array_o[i].y = 0;
	
	    append_cluster(&array_c[i], array_o[i]);
	    i++;
    }
    *arr = array_c;
    fclose(fp);
    return count_of_lines;
}
/* Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk)
 * Tiskne se prvnich 'narr' shluku.
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
    int number_of_clusters;
    //pokud je argc vice jak 3 -> vypis chybu
    if(argc > 3)
    {
	    fprintf(stderr, "error: too many arguents\n");
	    return 1;
    }
    //pokud je argc 3 -> nacti hodnotu z argv[2]
    if(argc == 3)
    {
	//zkontroluj, jestli je argv[2] cislo
	    if(isdigit(*argv[2]))
	        number_of_clusters = atoi(argv[2]);
	    else
	    {
	        fprintf(stderr, "error: bad format of argv[2]\n");
	        return 1;
	    }

        if(atoi(argv[2]) != atof(argv[2]))
        {
            fprintf(stderr, "error: bad format of argv[2]\n");
            return 1;
        }
    }
    if(argc < 2)
    {
	    //free(clusters);
	    fprintf(stderr, "error: little argument\n");
	    return 1;
    }
    //pokud argc je 2 -> uzivatel nezadal druhy parametr -> proved program,
    //jako kdyby uzivatel zadal 1 cluster
    if(argc == 2)
	    number_of_clusters = 1;
    //pokud uzivatel zadal argv[2] mensi jak 0, tak nastav hodnotu na 1
    if(number_of_clusters == 0)
        number_of_clusters = 1;
    //pokud argc je mensi jak 2 -> vypis chybu a ukoci program
    
    char *filename = argv[1];
    //nacti clustery
    int number_of_rows = load_clusters(filename, &clusters);
    for(int i = 0; i < number_of_rows; i++)
    {
	    for(int y = i + 1; y < number_of_rows; y++)
	    {
	        if(clusters[i].obj->id == clusters[y].obj->id)
	        {
    		    for(int k = 0; k < number_of_rows; k++)
                {
        	        clear_cluster(&clusters[k]);
                }
                free(clusters);
		        fprintf(stderr, "error: in file are 2 or more same ids\n");
	     	    return 1;
	        }
	    }
    }
    //pokud se nepodari nacist soubor -> vypis chybu a ukonci program
    if(number_of_rows == 0)
    {
	    fprintf(stderr, "error: file not found\n");
	    return 1;
    }
    //pokud neni na prvnim radku v souboru co ma byt -> vypis chybu a ukonci program
    if(number_of_rows == -1)
    {
	    free(clusters);
	    fprintf(stdout, "error: bad format of filename\n");
	    return 1;
    }
    if(number_of_rows == -2)
    {
        free(clusters);
	    fprintf(stderr, "error: count is less then 0\n");
	    return 1;
    }
    //pokud je pocet clustru vetsi nez pocet radku, tak hod error
    if(number_of_clusters > number_of_rows)
    {	
    	for(int i = 0; i < number_of_rows; i++)
        {
            clear_cluster(&clusters[i]);
        }
	    free(clusters);
	    fprintf(stdout, "error: number of clusters is bigger than number of rows in file\n");
	    return 1;
    }
    while(number_of_rows > number_of_clusters)
    {
	    int position_of_first_neighbour, position_of_second_neighbour;
	    //vyhledani nejblizsich sousedu
    	find_neighbours(clusters, number_of_rows, &position_of_first_neighbour, &position_of_second_neighbour);
	    //spojeni clusteru
	    merge_clusters(&clusters[position_of_first_neighbour], &clusters[position_of_second_neighbour]);
	    //odstraneni objktu z clusteru
	    number_of_rows = remove_cluster(clusters, number_of_rows, position_of_second_neighbour);		
    }
    //vypis clusteru
    print_clusters(clusters, number_of_rows);
    //vycisteni pameti
    for(int i = 0; i < number_of_rows; i++)
    {
        clear_cluster(&clusters[i]);
    }
    free(clusters);
    return 0;

}