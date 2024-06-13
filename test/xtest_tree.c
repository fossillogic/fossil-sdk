/*
==============================================================================
Author: Michael Gene Brockus (Dreamer)
Email: michaelbrockus@gmail.com
Organization: Fossil Logic
Description: 
    This file is part of the Fossil Logic project, where innovation meets
    excellence in software development. Michael Gene Brockus, also known as
    "Dreamer," is a dedicated contributor to this project. For any inquiries,
    feel free to contact Michael at michaelbrockus@gmail.com.
==============================================================================
*/
#include "fossil/structures/tree.h" // lib source code

#include <fossil/unittest.h>   // basic test tools
#include <fossil/xassume.h> // extra asserts

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// placeholder

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(test_tree_create_and_erase) {
    fossil_tree_t* tree = fossil_tree_create(TOFU_INT_TYPE);
    
    // Check if the tree is created with the expected values
    ASSUME_NOT_CNULL(tree);
    ASSUME_ITS_CNULL(tree->root);
    ASSUME_ITS_EQUAL_I32(TOFU_INT_TYPE, tree->tree);

    fossil_tree_erase(tree);

    // Check if the tree is erased
    ASSUME_ITS_CNULL(tree->root);
    ASSUME_ITS_CNULL(tree);
}

FOSSIL_TEST(test_tree_insert_and_search) {
    fossil_tree_t* tree = fossil_tree_create(TOFU_INT_TYPE);

    // Insert some elements
    fossil_tofu_t element1 = { TOFU_INT_TYPE, { .int_type = 42 } };
    fossil_tofu_t element2 = { TOFU_INT_TYPE, { .int_type = 10 } };
    fossil_tofu_t element3 = { TOFU_INT_TYPE, { .int_type = 5 } };

    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_insert(tree, element1));
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_insert(tree, element2));
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_insert(tree, element3));

    // Search for elements
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_search(tree, element1));
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_search(tree, element2));
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_search(tree, element3));

    // Search for non-existing element
    fossil_tofu_t nonExistingElement = { TOFU_INT_TYPE, { .int_type = 100 } };
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_INDEX_OUT_OF_BOUNDS, fossil_tree_search(tree, nonExistingElement));

    fossil_tree_erase(tree);
}

FOSSIL_TEST(test_tree_remove) {
    fossil_tree_t* tree = fossil_tree_create(TOFU_INT_TYPE);

    // Insert some elements
    fossil_tofu_t element1 = { TOFU_INT_TYPE, { .int_type = 42 } };
    fossil_tofu_t element2 = { TOFU_INT_TYPE, { .int_type = 10 } };
    fossil_tofu_t element3 = { TOFU_INT_TYPE, { .int_type = 5 } };

    fossil_tree_insert(tree, element1);
    fossil_tree_insert(tree, element2);
    fossil_tree_insert(tree, element3);

    // Remove an element
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_remove(tree, element2));

    // Search for removed and remaining elements
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_INDEX_OUT_OF_BOUNDS, fossil_tree_search(tree, element2));
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_search(tree, element1));
    ASSUME_ITS_EQUAL_I32(FOSSIL_TOFU_ERROR_OK, fossil_tree_search(tree, element3));

    fossil_tree_erase(tree);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(test_tree_group) {
    ADD_TEST(test_tree_create_and_erase);
    ADD_TEST(test_tree_insert_and_search);
    ADD_TEST(test_tree_remove);
} // end of func
