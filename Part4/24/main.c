#include <stdlib.h>
#include <stdio.h>

typedef struct item
{
	char *str;
	int str_len;
	char *file_name;
} item;

int str_len(const char *str)
{
	int i = 0;
	for (; str[i]; ++i)
		;
	return i;
}
void concatinate_string(char **dest, const char *src)
{
	char *buf = *dest;
	int src_size = str_len(src);
	int buf_size = 0;
	if (buf)
		buf_size = str_len(buf);
	char *new_buf = (char *)malloc(sizeof(char) * (src_size + buf_size + 1));
	int i = 0;

	for (; i < buf_size; ++i)
	{
		new_buf[i] = buf[i];
	}
	for (int j = 0; j <= src_size; ++j, ++i)
	{
		new_buf[i] = src[j];
	}
	if (buf)
		free(buf);
	*dest = new_buf;
	return;
}
void search_and_print_most_loger_string(int names_count, char **file_names)
{
	int buf_size = 10;
	item **items = (item **)malloc(sizeof(item) * names_count);
	char *buf = (char *)malloc(sizeof(char) * buf_size);
	int most_longest = 0;
	for (int i = 0; i < names_count; ++i)
	{
		int len = 0, max_len = 0, readed = 0;
		items[i] = NULL;
		FILE *infile;
		if (!(infile = fopen(file_names[i], "r")))
			continue;

		item *element;
		if(!(element = (item *)malloc(sizeof(item))))
			continue;
		element->str = NULL;
		element->str_len = 0;
		element->file_name = file_names[i];
		items[i] = element;
		
		char *accum = NULL;
		while (fgets(buf, buf_size, infile))
		{
			readed = str_len(buf);
			len += readed;
			if (len > max_len)
				max_len = len;
			
			concatinate_string(&accum, buf);
			if (buf[readed - 1] == '\n')
			{
				if(most_longest < max_len)
					most_longest = max_len;
				if (element->str_len < max_len)
				{
					free(element->str);
					element->str = NULL;
					concatinate_string(&element->str, accum);
					element->str_len = max_len;
				}
				
				free(accum);
				accum = NULL;
				len = 0;
			}
			readed = 0;
		}
		
		
		fclose(infile);
	}

	for (int i = 0; i < names_count; ++i)
	{
		if (!items[i])
		 	continue;
		if(items[i]->str_len == most_longest)
			putchar('*');
		printf("%s: %s", items[i]->file_name, items[i]->str);
		free(items[i]->str);
		free(items[i]);
	}
	free(items);
	free(buf);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;

	search_and_print_most_loger_string(argc - 1, &argv[1]);

	return 0;
}