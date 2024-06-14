#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;
    char *surname;
} Structure;

int compareInt(const void *int1, const void *int2) {
    return *(int *)int1 - *(int *)int2;
}

int compareChar(const void *char1, const void *char2) {
    return *(char *)char1 - *(char *)char2;
}

int compareFloat(const void *float1, const void *float2) {
    float diff = *(float *)float1 - *(float *)float2;
    if (diff < -1e-6) return -1;
    else if (diff > 1e-6) return 1;
    else return 0;
}

int compareDouble(const void *double1, const void *double2) {
    return (*(double *)double1 - *(double *)double2);
}

int compareStruct(const void *struct1, const void *struct2) {
    Structure *s1 = (Structure *)struct1;
    Structure *s2 = (Structure *)struct2;

    if (s1->id != s2->id) {
        return s1->id - s2->id;
    }

    return strcmp(s1->surname, s2->surname);
}

int compareString(const void *string1, const void *string2) {
    const char *str1 = (const char *)string1;
    const char *str2 = (const char *)string2;
    return strcmp(str1, str2);
}



void* bsearch(const void *key, const void *base, size_t num, size_t size, int (*compare)(const void *, const void *)) {
    if (num == 0) {
        return NULL;
    }

    size_t mid_index = num / 2;
    char* mid = (char *)base + mid_index * size;

    int cmp = compare(key, mid);
    if (cmp == 0) {
        return (void*)mid;
    } else if (cmp > 0) {
        return bsearch(key, mid + size, num - mid_index - 1, size, compare);
    } else {
        return bsearch(key, base, mid_index, size, compare);
    }
}

void testForInt(int num) {
    int array[num];
    for (int i = 0; i < num; i++) {
        array[i] = i + 1;
    }
    int expected_tests = num, found = 0, notFound = 0;
    int int_size = sizeof(int);
    int nonexistent = num + 1;

    for (int i = 1; i <= num; i++) {
        int *result = (int *)bsearch(&i, array, num, int_size, compareInt);
        if (result) {
            found += *result == i;  
        } else {
            found += !array[i - 1];
        }

        result = (int *)bsearch(&nonexistent, array, num, int_size, compareInt);
        if (result) {
            notFound += *result == 0;
        } else {
            notFound += 1;
        }
    }
    if (found == expected_tests) {
        printf("Passed %d tests for integers (existing value).\n", num);
    } else {
        printf("Failed %d tests for integers (existing value).\n", num);
    }
    if (notFound == expected_tests) {
        printf("Passed %d tests for integers (nonexisting value).\n", num);
    } else {
        printf("Failed %d tests for integers (nonexisting value).\n", num);
    }
}

void testForFloat(int num) {
    float array[num];
    for (int i = 0; i < num; i++) {
        array[i] = i + 1;
    }
    int expected_tests = num, found = 0, notFound = 0;
    int float_size = sizeof(float);
    float nonexistent = num + 1;

    for (int i = 1; i <= num; i++) {
        float value = (float)i;
        float *result = (float *)bsearch(&value, array, num, float_size, compareFloat);
        if (result) {
            found += *result == value;
        } else {
            found += !array[i - 1];
        }

        result = (float *)bsearch(&nonexistent, array, num, float_size, compareFloat);
        if (result) {
            notFound += *result == 0;
        } else {
            notFound += 1;
        }
    }
    if (found == expected_tests) {
        printf("Passed %d tests for floats (existing value).\n", num);
    } else {
        printf("Failed %d tests for floats (existing value).\n", num);
    }
    if (notFound == expected_tests) {
        printf("Passed %d tests for floats (nonexisting value).\n", num);
    } else {
        printf("Failed %d tests for floats (nonexisting value).\n", num);
    }
}

void testForDouble(int num) {
    double array[num];
    for (int i = 0; i < num; i++) {
        array[i] = i + 1;
    }
    int expected_tests = num, found = 0, notFound = 0;
    int double_size = sizeof(double);
    double nonexistent = num + 1;

    for (int i = 1; i <= num; i++) {
        double value = (double)i;
        double *result = (double *)bsearch(&value, array, num, double_size, compareDouble);
        if (result) {
            found += *result == value;
        } else {
            found += !array[i - 1];
        }

        result = (double *)bsearch(&nonexistent, array, num, double_size, compareDouble);
        if (result) {
            notFound += *result == 0;
        } else {
            notFound += 1;
        }
    }
    if (found == expected_tests) {
        printf("Passed %d tests for doubles (existing value).\n", num);
    } else {
        printf("Failed %d tests for doubles (existing value).\n", num);
    }
    if (notFound == expected_tests) {
        printf("Passed %d tests for doubles (nonexisting value).\n", num);
    } else {
        printf("Failed %d tests for doubles (nonexisting value).\n", num);
    }
}

void testForChar(int num) {
    char array[num];
    for (int i = 0; i < num; i++) {
        array[i] = 'a' + i;
    }
    int expected_tests = num, found = 0, notFound = 0;
    int char_size = sizeof(char);

    for (int i = 0; i < num; i++) {
        char value = 'a' + i;
        char *result = (char *)bsearch(&value, array, num, char_size, compareChar);
        if (result) {
            found += *result == value;
        } else {
            found += !array[i];
        }
        char nonexistent = 'a' - (i + 1);
        result = (char *)bsearch(&nonexistent, array, num, char_size, compareChar);
        if (result) {
            notFound += *result == 0;
        } else {
            notFound += 1;
        }
    }

    if (found == expected_tests) {
        printf("Passed %d tests for chars (existing value).\n", num);
    } else {
        printf("Failed %d tests for chars (existing value).\n", num);
    }
    if (notFound == expected_tests) {
        printf("Passed %d tests for chars (nonexisting value).\n", num);
    } else {
        printf("Failed %d tests for chars (nonexisting value).\n", num);
    }
}

void testForStruct() {
    Structure array[5] = {{1, "Messi"}, {2, "Ronaldo"}, {3, "Neymar"}, {4, "Mbappe"}, {5, "Lewandowski"}};
    int expected_tests = 5, found = 0, notFound = 0;
    int struct_size = sizeof(Structure);

    for (int i = 0; i < 5; i++) {
        Structure value = {i + 1, array[i].surname};
        Structure *result = (Structure *)bsearch(&value, array, 5, struct_size, compareStruct);
        if (result) {
            found += result->id == value.id;
        } else {
            found += !array[i].id;
        }
        Structure Milik = {i + 6, "Milik"};
        result = (Structure *)bsearch(&Milik, array, 5, struct_size, compareStruct);
        if (result) {
            notFound += result->id == 0;
        } else {
            notFound += 1;
        }
    }

    if (found == expected_tests) {
        printf("Passed %d tests for structs (existing value).\n", 5);
    } else {
        printf("Failed %d tests for structs (existing value).\n", 5);
    }
    if (notFound == expected_tests) {
        printf("Passed %d tests for structs (nonexisting value).\n", 5);
    } else {
        printf("Failed %d tests for structs (nonexisting value).\n", 5);
    }
}

void testForStrings() {
    const char *array[5] = {"Lewandowski", "Mbappe", "Messi", "Neymar", "Ronaldo"};
	int expected_tests = 5, found = 0, notFound = 0;
	int string_size = sizeof(char *);

	qsort(array, 5, string_size, compareString);
	for (int i = 0; i < 5; i++) {
		const char *value = array[i];
		const char **result = (const char **)bsearch(&value, array, 5, string_size, compareString);

		if (result) {
			found += strcmp(*result, value) == 0;
		} else {
			found += !array[i];
		}
		const char *nonexistent = "Milik";
		result = (const char **)bsearch(&nonexistent, array, 5, string_size, compareString);
		if (result) {
			notFound += strcmp(*result, "") == 0;
		} else {
			notFound += 1;
		}
	}

	if (found == expected_tests) {
		printf("Passed %d tests for strings (existing value).\n", 5);
	} else {
		printf("Failed %d tests for strings (existing value).\n", 5);
	}
	if (notFound == expected_tests) {
		printf("Passed %d tests for strings (nonexisting value).\n", 5);
	} else {
		printf("Failed %d tests for strings (nonexisting value).\n", 5);
	}
}

int main() {
    testForInt(10);
    testForFloat(10);
    testForDouble(10);
    testForChar(30);
    testForStruct();
	testForStrings();
    return 0;
}
