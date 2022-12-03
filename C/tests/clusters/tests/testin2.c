#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

struct obj_t
{
	int id;
	float x;
	float y;
};

struct cluster_t
{
	int size;
	int capacity;
	struct obj_t *obj;
};

void init_cluster(struct cluster_t *c, int cap)
{
printf("%p\n", c);
	/*assert(c != NULL);
	assert(cap >= 0);*/
	c = malloc(sizeof(*c));
	if (!c)
	{
		fprintf(stderr, "Malloc failed\n");
		return;
	}
	c->size = 0;
debug("1");
	c->capacity = cap;
debug("2");
	c->obj = NULL;
debug("3");
printf("Size: %d, Capacity: %d, Obj: %p\n", c->size, c->capacity, c->obj);
	return;
}

void print_cluster(struct cluster_t *c)
{
	//TUTO FUNKCI NEMENTE
	for (int i = 0; i < c->size; i++)
	{
		if (i) putchar(' ');
		printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
	}
	putchar('\n');
}

int load_clusters(char *filename, struct cluster_t **arr)
{
	FILE *input;
	char first_line[] = "count=";
	char line[100];
	input = fopen(filename, "r");
	if (input == NULL)
	{
		return 0;
	}
	int sizeF = sizeof(first_line) / sizeof(char);
	int size = sizeof(line) / sizeof(char);
	fgets(line, sizeF, input);
	for (int i = 0; i < sizeF; i++)
	{
		if (first_line[i] != line[i])
	        {
			return 0;
		}
	}
	fgets(line, size, input);
	int line_cnt = atoi(line);
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
	
	struct cluster_t c_arr[line_cnt];
	struct obj_t o_arr[line_cnt];
	for (int i = 0; i < line_cnt; i++)
	{
		o_arr[i].id = array[i][0];
		o_arr[i].x = array[i][1];
		o_arr[i].y = array[i][2];
	/*	fprintf(stderr, "%d\n", o_arr[i]->id);*/
		c_arr[i].size = 1;
		c_arr[i].capacity = 1;
		c_arr[i].obj = &o_arr[i];
		fprintf(stderr, "c_arr[i]: %p,Obj: %p, o_arr: %p\n", &c_arr[i], c_arr[i].obj, &o_arr[i]);
	/*	c_arr[i].obj = o_arr[i];*/
	/*	fprintf(stderr, "1\n");
		c_arr[i].obj->id = array[i][0];
		fprintf(stderr, "2\n");
		c_arr[i].obj->x = array[i][1];
		c_arr[i].obj->y = array[i][2];
	*/	//arr[i] = c_arr;
	}
//ulozeno v clusterech, ted jen poslat na **arr
fprintf(stderr, "c_arr: %p\n", c_arr);	
	*arr = c_arr;
fprintf(stderr, "c_arr[0]: %p\n", &c_arr[0]);	
	print_cluster(&c_arr[0]);
	print_cluster(&c_arr[1]);
	fclose(input);
	return line_cnt;
}

int main()
{
	//struct cluster_t *clusters;
	//ukazatel na cluster
	struct cluster_t *arr[100];
	char file[] = "objekty.txt";
	int x = load_clusters(file, (arr));
fprintf(stderr, "arr: %p\n", arr[0]);
	print_cluster(arr[0]);
	printf("\n");
	for (int i = 0; i < 20; i++)
	{
		//int *x arr[0]->obj
		//printf("Add of ID: %p\n", arr[0]/*->obj*/ + i/*i * sizeof(struct cluster_t)*/);
		//printf("ID: %d\n", arr[0]->obj->id);
		print_cluster(arr[0] + i);
	}
	printf("%d\n", x);
	/*for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}*/
/**	struct cluster_t cluster;
	int y = cluster.size = 25;
	printf("acas%d\n", y);
debug("Ahoj");
printf("%p\n", &cluster);
	init_cluster(&cluster, 1);
printf("%p\n", &cluster);
debug("Ahoj2");
	int x = cluster.size;
	printf("ah%d\n", x);
printf("Size: %d, Capacity: %d, Obj: %p\n", cluster.size, cluster.capacity, cluster.obj);
**/	/*cluster->obj->id = 10;
	cluster->obj->x = 2;
	cluster->obj->y = 3;
	cluster->size++;*/
	//print_cluster(cluster);
	//printf("Size: %d\nCapacity: %d\n", cluster->size, cluster->capacity);
	return 0;
}
