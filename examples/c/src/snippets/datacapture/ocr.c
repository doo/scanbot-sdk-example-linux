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

    ec = scanbotsdk_line_as_scanbotsdk_ocr_element(line, &el);
    if (ec != SCANBOTSDK_OK) return ec;

    print_ocr_element_text(el, "Line", 2);

    size_t word_count = 0;
    ec = scanbotsdk_line_get_words_size(line, &word_count);
    if (ec != SCANBOTSDK_OK || word_count == 0) return ec;

    scanbotsdk_word_t **words = calloc(word_count, sizeof(*words));
    if (!words) return SCANBOTSDK_ERROR_UNKNOWN_ERROR;

    ec = scanbotsdk_line_get_words(line, words, word_count);
    for (size_t k = 0; k < word_count; k++) {
        process_word(words[k]);
    }

    free(words);
    return ec;
}

static scanbotsdk_error_code_t process_block(scanbotsdk_block_t *block) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;
    scanbotsdk_ocr_element_t *el = NULL;

    ec = scanbotsdk_block_as_scanbotsdk_ocr_element(block, &el);
    if (ec != SCANBOTSDK_OK) return ec;

    print_ocr_element_text(el, "Block", 0);

    size_t line_count = 0;
    ec = scanbotsdk_block_get_lines_size(block, &line_count);
    if (ec != SCANBOTSDK_OK || line_count == 0) return ec;

    scanbotsdk_line_t **lines = calloc(line_count, sizeof(*lines));
    if (!lines) return SCANBOTSDK_ERROR_UNKNOWN_ERROR;

    ec = scanbotsdk_block_get_lines(block, lines, line_count);
    for (size_t j = 0; j < line_count; j++) {
        process_line(lines[j]);
    }
    
    free(lines);
    return ec;
}

scanbotsdk_error_code_t print_ocr_result(scanbotsdk_page_t *page) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    size_t block_count = 0;
    ec = scanbotsdk_page_get_blocks_size(page, &block_count);
    if (ec != SCANBOTSDK_OK) return ec;

    if (block_count == 0) { printf("No text found.\n"); return SCANBOTSDK_OK;}

    scanbotsdk_block_t **blocks = calloc(block_count, sizeof(*blocks));
    if (!blocks) return SCANBOTSDK_ERROR_UNKNOWN_ERROR;

    ec = scanbotsdk_page_get_blocks(page, blocks, block_count);
    for (size_t i = 0; i < block_count; i++) {
        process_block(blocks[i]);
    }
    
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