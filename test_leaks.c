/* Simple test to verify major allocations are working properly */
#include "headers/ft_utils.h"
#include "headers/ft_error.h"
#include <stdio.h>

int main() {
    // Test basic utility functions
    printf("Testing split function...\n");
    char **test_split = ft_split("hello,world,test", ",");
    if (test_split) {
        printf("Split test: %s %s %s\n", test_split[0], test_split[1], test_split[2]);
        ft_strvfree(test_split);
        printf("Split test freed successfully\n");
    }
    
    // Test hashmap
    printf("Testing hashmap...\n");
    t_hashmap *map = ft_hashmap_new();
    if (map) {
        ft_hashmap_set(map, "test", ft_strdup("value"), free);
        char *val = (char*)ft_hashmap_get_value(map, "test");
        if (val) {
            printf("Hashmap test: %s\n", val);
        }
        ft_hashmap_destroy(map);
        printf("Hashmap destroyed successfully\n");
    }
    
    // Test string operations
    printf("Testing string operations...\n");
    char *str1 = ft_strdup("Hello");
    char *str2 = ft_strndup("World!", 5);
    char *joined = ft_strjoin(str1, str2);
    printf("String operations: '%s'\n", joined);
    free(str1);
    free(str2);
    free(joined);
    
    // Test list operations
    printf("Testing list operations...\n");
    t_list *list = NULL;
    ft_list_add(&list, ft_strdup("item1"), free);
    ft_list_add(&list, ft_strdup("item2"), free);
    printf("List size: %zu\n", ft_list_size(list));
    ft_list_destroy(&list);
    printf("List destroyed successfully\n");
    
    printf("All tests completed successfully - check for memory leaks\n");
    return 0;
}
