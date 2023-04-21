# Documentation

This document contains snippets of information which were useful during the development of this library.

## Pushing records to sections

Matrix below describes which types of records can be pushed to which types of sections.

Legend:
- `+` = can be pushed
  * `(1)` = can be pushed if section is empty DATA section
- `-` = can not be pushed
- `REC` = record
- `SEC` = section
- `DAT` = DATA record/section
- `EOF` = END_OF_FILE record/section
- `ESA` = EXTENDED_SEGMENT_ADDRESS record/section
- `SSA` = START_SEGMENT_ADDRESS record/section
- `ELA` = EXTENDED_LINEAR_ADDRESS record/section
- `SLA` = START_LINEAR_ADDRESS record/section

| REC \ SEC  | DAT  | EOF  | ESA  | SSA  | ELA  | SLA  |
|:-----------|:-----|:-----|:-----|:-----|:-----|:-----|
| DAT        | +    | -    | +    | -    | +    | -    |
| EOF        | +(1) | -    | -    | -    | -    | -    |
| ESA        | +(1) | -    | -    | -    | -    | -    |
| SSA        | +(1) | -    | -    | -    | -    | -    |
| ELA        | +(1) | -    | -    | -    | -    | -    |
| SLA        | +(1) | -    | -    | -    | -    | -    |

## Adding/pushing sections to groups

Matrix below describes which types of sections can be added/pushed to which types of groups.

Legend:
- `+` = can be added/pushed
  * `(1)` = can be added/pushed to the group only if this section type does not already exist in the group and to the end of the group only
  * `(2)` = can be added/pushed to the group only if this section type does not already exist in the group
  * `(3)` = can be added/pushed to the group only if the group does not have a DATA section
- `-` = can not be added/pushed
- `SEC` = section
- `GRO` = group (of sections)
- `DAT` = SECTION_DATA section
- `EOF` = SECTION_END_OF_FILE section
- `ESA` = SECTION_EXTENDED_SEGMENT_ADDRESS section
- `SSA` = SECTION_START_SEGMENT_ADDRESS section
- `ELA` = SECTION_EXTENDED_LINEAR_ADDRESS section
- `SLA` = SECTION_START_LINEAR_ADDRESS section

| SEC \ GRO  | I8HEX  | I16HEX | I32HEX |
|:-----------|:-------|:-------|:-------|
| DAT        | +(3)   | -      | -      |
| EOF        | +(1)   | +(1)   | +(1)   |
| ESA        | +(3)   | +      | -      |
| SSA        | +(3)   | +(2)   | -      |
| ELA        | +(3)   | -      | +      |
| SLA        | +(3)   | -      | +(2)   |
