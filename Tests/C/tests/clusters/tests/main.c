#include <stdio.h>
#include <stdlib.h>

struct set
{
    int size;
    int *data;
};

struct set create_empty()
{
    struct set s = {0, NULL};
    return s;
}

void resize(struct set *s, int size)
{
    if (s->size == 0)
    {
        s->data = malloc(size * sizeof(int));
        if (s->data == NULL)
        {
            s->size = 0;
            return;
        }
    }
    else
    {
        s->data = realloc(s->data, size * sizeof(int));
        if (s->data == NULL)
        {
            s->size = 0;
            return;
        }
    }
    s->size = size;
}

void add(struct set *s, int elem)
{
    if (contains(s, elem))
        return;
    resize(s, s->size + 1);
    s->data[s->size - 1] = elem;
}

int contains(struct set *s, int elem)
{
    for (int i = 0; i < s->size; i++)
    {
        if (s->data[i] == elem)
            return 1;
    }
    return 0;
}

void print(struct set *s)
{
    printf("{");
    for (int i = 0; i < s->size; i++)
    {
        printf("%d", s->data[i]);
        if (i < s->size - 1)
            printf(", ");
    }
    printf("}\n");
}

struct set set_union(struct set *s1, struct set *s2)
{
    struct set s = create_empty();
    for (int i = 0; i < s1->size; i++)
        add(&s, s1->data[i]);
    for (int i = 0; i < s2->size; i++)
        add(&s, s2->data[i]);
    return s;
}

int main()
{
    struct set s1 = create_empty();
    add(&s1, 0);
    add(&s1, 1);
    add(&s1, 2);
    print(&s1);

    struct set s2 = create_empty();
    add(&s2, 1);
    add(&s2, 2);
    add(&s2, 3);
    print(&s2);

    struct set s = create_empty();
    s = set_union(&s1, &s2);
    print(&s);

    return 0;
}
