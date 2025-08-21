#include <stdio.h>
#include <stdlib.h>

#include <snippets/datacapture/ocr.h>
#include <utils/utils.h>
scanbotsdk_error_code_t print_ocr_result(scanbotsdk_page_t *page) {
    scanbotsdk_error_code_t ec = SCANBOTSDK_OK;

    scanbotsdk_block_t **blocks = NULL;
    scanbotsdk_line_t **lines = NULL;
    scanbotsdk_word_t **words = NULL;

    size_t block_count = 0;
    ec = scanbotsdk_page_get_blocks_size(page, &block_count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_blocks_size: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    if (block_count == 0) {
        printf("No text found.\n");
        goto cleanup;
    }

    blocks = calloc(block_count, sizeof(scanbotsdk_block_t*));
    if (!blocks) { fprintf(stderr, "alloc blocks failed\n"); ec = SCANBOTSDK_ERROR_UNKNOWN_ERROR; goto cleanup; }

    ec = scanbotsdk_page_get_blocks(page, blocks, block_count);
    if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_blocks: %d: %s\n", ec, error_message(ec)); goto cleanup; }

    for (size_t i = 0; i < block_count; i++) {
        scanbotsdk_block_t *block = blocks[i];
        scanbotsdk_ocr_element_t *block_el = NULL;
        ec = scanbotsdk_block_as_scanbotsdk_ocr_element(block, &block_el);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "block_as_ocr: %d: %s\n", ec, error_message(ec)); continue; }

        const char *block_text = NULL;
        scanbotsdk_ocr_element_get_text(block_el, &block_text);
        printf("Block: \"%s\"\n", block_text);
        
        size_t line_count = 0;
        ec = scanbotsdk_block_get_lines_size(block, &line_count);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_lines_size: %d: %s\n", ec, error_message(ec)); continue; }

        lines = calloc(line_count, sizeof(scanbotsdk_line_t*));
        if (!lines) { fprintf(stderr, "alloc lines failed\n"); ec = SCANBOTSDK_ERROR_UNKNOWN_ERROR; goto cleanup; }

        ec = scanbotsdk_block_get_lines(block, lines, line_count);
        if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_lines: %d: %s\n", ec, error_message(ec)); free(lines); lines = NULL; continue; }

        for (size_t j = 0; j < line_count; j++) {
            scanbotsdk_line_t *line = lines[j];
            scanbotsdk_ocr_element_t *line_el = NULL;
            ec = scanbotsdk_line_as_scanbotsdk_ocr_element(line, &line_el);
            if (ec != SCANBOTSDK_OK) { fprintf(stderr, "line_as_ocr: %d: %s\n", ec, error_message(ec)); continue; }

            const char *line_text = NULL;
            ec = scanbotsdk_ocr_element_get_text(line_el, &line_text);
            if (ec == SCANBOTSDK_OK && line_text) {
                printf("  Line: \"%s\"\n", line_text);
            }

            size_t word_count = 0;
            ec = scanbotsdk_line_get_words_size(line, &word_count);
            if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_words_size: %d: %s\n", ec, error_message(ec)); continue; }

            words = calloc(word_count, sizeof(scanbotsdk_word_t*));
            if (!words) { fprintf(stderr, "alloc words failed\n"); ec = SCANBOTSDK_ERROR_UNKNOWN_ERROR; goto cleanup; }

            ec = scanbotsdk_line_get_words(line, words, word_count);
            if (ec != SCANBOTSDK_OK) { fprintf(stderr, "get_words: %d: %s\n", ec, error_message(ec)); free(words); words = NULL; continue; }

            for (size_t k = 0; k < word_count; k++) {
                scanbotsdk_word_t *word = words[k];
                scanbotsdk_ocr_element_t *word_el = NULL;
                ec = scanbotsdk_word_as_scanbotsdk_ocr_element(word, &word_el);
                if (ec != SCANBOTSDK_OK) { fprintf(stderr, "word_as_ocr: %d: %s\n", ec, error_message(ec)); continue; }

                const char *word_text = NULL;
                ec = scanbotsdk_ocr_element_get_text(word_el, &word_text);
                if (ec == SCANBOTSDK_OK && word_text) {
                    printf("    Word: \"%s\"\n", word_text);
                }
            }

            free(words);
        }

        free(lines);
    }

cleanup:
    if (blocks) free(blocks);
    if (lines) free(lines);
    if (words) free(words);
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