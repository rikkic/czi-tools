#ifndef _ZEISS_H
#define _ZEISS_H

#include <stdint.h>

/* data structures */

/* packed structures are used as an optimisation for reading in structures from the CZI file */
#define PACKED __attribute__ ((__packed__))

/* segment type */
enum czi_seg_t {
    ZISRAWFILE,
    ZISRAWDIRECTORY,
    ZISRAWSUBBLOCK,
    ZISRAWMETADATA,
    ZISRAWATTACH,
    ZISRAWATTDIR,
    DELETED,
    UNKNOWN
};

/* CZI segment header */
struct czi_seg_header {
    char name[16];
    uint64_t allocated_size;
    uint64_t used_size;
} PACKED;

/* CZI ZISRAWFILE segment */
struct czi_zrf {
    uint32_t major;
    uint32_t minor;
    uint32_t reserved1;
    uint32_t reserved2;
    char primary_file_guid[16];
    char file_guid[16];
    uint32_t file_part;
    uint64_t directory_position;
    uint64_t metadata_position;
    uint32_t update_pending;
    uint64_t attachment_directory_position;
} PACKED;

/* CZI ZISRAWMETADATA segment */
struct czi_metadata {
    uint32_t xml_size;
    uint32_t attachment_size;
    char padding[248];
} PACKED;

/* SubBlock dimension entry */
struct czi_subblock_dimentry {
    char dimension[4];
    int32_t start;
    uint32_t size;
    float start_coordinate;
    uint32_t stored_size;
} PACKED;

/* SubBlock directory entry */
struct czi_subblock_direntry {
    char schema[2];
    uint32_t pixel_type;
    uint64_t file_position;
    uint32_t file_part;
    uint32_t compression;
    char pyramid_type;
    char reserved1;
    char reserved2[4];
    uint32_t dimension_count;
    /* a directory entry is followed by a list of dimension entries */
} PACKED;

/* CZI ZISRAWSUBBLOCK segment */
struct czi_subblock {
    uint32_t metadata_size;
    uint32_t attachment_size;
    uint64_t data_size;
    /* a subblock is followed by a directory entry */
    struct czi_subblock_direntry dir_entry;
} PACKED;

/* CZI ZISRAWDIRECTORY segment -- subblock directory information */
struct czi_directory {
    uint32_t entry_count;
    char reserved[124];
    /* a directory block is followed by a list of directory entries */
} PACKED;

/* Attachment directory entry */
struct czi_attach_entry {
    char schema_type[2];
    char reserved[10];
    uint64_t file_position;
    int32_t file_part;
    char content_guid[16];
    char content_file_type[8];
    char name[80];
} PACKED;

/* CZI ZISRAWATTACH segment */
struct czi_attach {
    uint32_t data_size;
    char reserved1[12];
    struct czi_attach_entry att_entry;
    char reserved2[112];
} PACKED;

/* CZI ZISRAWATTDIR segment */
struct czi_attach_dir {
    uint32_t entry_count;
    char reserved[252];
    /* an attachment directory is followed by a list of attachment entries */
} PACKED;

/* support functions */

enum czi_seg_t czi_getsegid(struct czi_seg_header *);

#endif /* _ZEISS_H */
