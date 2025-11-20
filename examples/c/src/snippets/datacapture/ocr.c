#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/ocr.h>
#include <utils/utils.h>

static void print_ocr_element_text(scanbotsdk_ocr_element_t *el, const char *label, int indent) {
    const char *text = NULL;
    scanbotsdk_ocr_element_get_text(el, &text);

    for (int i = 0; i < indent; i++) printf(" ");
    printf("%s: \"%s\"\n", label, text ? text : "");
}

static scanbotsdk_error_code_t process_word(scanbotsdk_word_t *word) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_ocr_element_t *el = NULL;

    ec = scanbotsdk_word_as_scanbotsdk_ocr_element(word, &el);
    if (ec != SCANBOTSDK_OK) return ec;

    print_ocr_element_text(el, "Word", 4);
    return ec;
}

static scanbotsdk_error_code_t process_line(scanbotsdk_line_t *line) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_ocr_element_t *el = NULL;
    scanbotsdk_word_t **words = NULL;
    size_t word_count = 0;

    ec = scanbotsdk_line_as_scanbotsdk_ocr_element(line, &el);
    if (ec != SCANBOTSDK_OK) goto cleanup;

    print_ocr_element_text(el, "Line", 2);

    ec = scanbotsdk_line_get_words_size(line, &word_count);
    if (ec != SCANBOTSDK_OK || word_count == 0) goto cleanup;

    words = calloc(word_count, sizeof(*words));
    if (!words) { ec = SCANBOTSDK_ERROR_UNKNOWN_ERROR; goto cleanup; }

    ec = scanbotsdk_line_get_words(line, words, word_count);
    if (ec != SCANBOTSDK_OK) goto cleanup;

    for (size_t k = 0; k < word_count; k++) {
        ec = process_word(words[k]);
        if (ec != SCANBOTSDK_OK) goto cleanup;
    }

cleanup:
    free(words);
    return ec;
}

static scanbotsdk_error_code_t process_block(scanbotsdk_block_t *block) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_ocr_element_t *el = NULL;
    scanbotsdk_line_t **lines = NULL;         // declare before gotos
    size_t line_count = 0;

    ec = scanbotsdk_block_as_scanbotsdk_ocr_element(block, &el);
    if (ec != SCANBOTSDK_OK) goto cleanup;

    print_ocr_element_text(el, "Block", 0);

    ec = scanbotsdk_block_get_lines_size(block, &line_count);
    if (ec != SCANBOTSDK_OK || line_count == 0) goto cleanup;

    lines = calloc(line_count, sizeof(*lines));
    if (!lines) { ec = SCANBOTSDK_ERROR_UNKNOWN_ERROR; goto cleanup; }

    ec = scanbotsdk_block_get_lines(block, lines, line_count);
    if (ec != SCANBOTSDK_OK) goto cleanup;

    for (size_t j = 0; j < line_count; j++) {
        ec = process_line(lines[j]);
        if (ec != SCANBOTSDK_OK) goto cleanup;
    }

cleanup:
    free(lines);
    return ec;
}

static scanbotsdk_error_code_t print_ocr_result(scanbotsdk_page_t *page) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_block_t **blocks = NULL;
    size_t block_count = 0;

    ec = scanbotsdk_page_get_blocks_size(page, &block_count);
    if (ec != SCANBOTSDK_OK) goto cleanup;

    if (block_count == 0) {
        printf("No text found.\n");
        goto cleanup;
    }

    blocks = calloc(block_count, sizeof(*blocks));
    if (!blocks) { ec = SCANBOTSDK_ERROR_UNKNOWN_ERROR; goto cleanup; }

    ec = scanbotsdk_page_get_blocks(page, blocks, block_count);
    if (ec != SCANBOTSDK_OK) goto cleanup;

    for (size_t i = 0; i < block_count; i++) {
        ec = process_block(blocks[i]);
        if (ec != SCANBOTSDK_OK) goto cleanup;
    }

cleanup:
    free(blocks);
    return ec;
}

scanbotsdk_error_code_t ocr_run(scanbotsdk_image_t *image) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    
    scanbotsdk_ocr_engine_t *engine = NULL;
    scanbotsdk_page_t *result = NULL;

    ec = scanbotsdk_ocr_engine_create(&engine);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "engine_create: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    ec = scanbotsdk_ocr_engine_run(engine, image, &result);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "engine_run: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    print_ocr_result(result);

cleanup:
    scanbotsdk_ocr_engine_free(engine);
    scanbotsdk_page_free(result);
    return ec;
}