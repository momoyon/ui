#ifndef UI_H_
#define UI_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Remove Prefix
#ifdef UI_REMOVE_PREFIX
#define ASSERT UI_ASSERT 
#define UI_Vector2f Vector2f 
#define Layout_kind UI_Layout_kind 
#define Layout UI_Layout 
#define Layout_make UI_Layout_make 
#define Layout_available_pos UI_Layout_available_pos 
#define Layout_push_widget UI_Layout_push_widget 
#define Context UI_Context 
#define Context_make UI_Context_make 
#endif

/*
 * Stand-Alone UI library that can work with any graphics API.
 */

// Macros
#define UI_ASSERT(condition, message) do {\
		if (!(condition)) {\
			fprintf(stderr, "%s:%u:0: ASSERTION FAILED: %s '%s'", __FILE__, __LINE__, #condition, message);\
			exit(1);\
		}\
	} while (0)

// Struct: UI_Vector2f
typedef struct {
	float x, y;
} UI_Vector2f;

// Enum: UI_Layout_kind
typedef enum {
	UI_LAYOUT_KIND_VERT,
	UI_LAYOUT_KIND_HORZ,
	UI_LAYOUT_KIND_COUNT,
} UI_Layout_kind;

// Struct: UI_Layout
typedef struct {
	UI_Layout_kind kind;
	UI_Vector2f pos, size;
} UI_Layout;

// Methods of UI_Layout
UI_Layout UI_Layout_make(UI_Layout_kind kind);
UI_Vector2f  UI_Layout_available_pos(UI_Layout* layout);
void      UI_Layout_push_widget(UI_Layout* layout, UI_Vector2f size);

typedef struct {
	UI_Vector2f pos;
#define LAYOUTS_CAP 1024
	UI_Layout layouts[LAYOUTS_CAP];
	size_t    layouts_count;
} UI_Context;

// Methods of UI_Context
UI_Context UI_Context_make();

#endif

//////////////////////////////////////////////////
#ifdef UI_IMPLEMENTATION

// Methods of UI_Layout
UI_Layout UI_Layout_make(UI_Layout_kind kind) {
	UI_Layout l = {
		.kind = kind,
	};
	return l;
}

UI_Vector2f  UI_Layout_available_pos(UI_Layout* layout) {
	switch (layout->kind) {
		case UI_LAYOUT_KIND_VERT: {
						  return (UI_Vector2f) { layout->pos.x, layout->pos.y + layout->size.y };
					  } break;
		case UI_LAYOUT_KIND_HORZ: {
						  return (UI_Vector2f) { layout->pos.x + layout->size.x, layout->pos.y };
					  } break;
		case UI_LAYOUT_KIND_COUNT: 
		default: {
				 UI_ASSERT(0, "UNREACHABLE!");
			 } break;

	}
	return (UI_Vector2f) {0.f, 0.f};
}

void UI_Layout_push_widget(UI_Layout* layout, UI_Vector2f size) {
	switch (layout->kind) {
		case UI_LAYOUT_KIND_VERT: {
						  layout->size.x += size.x;
						  layout->size.y = fmaxf(layout->size.y, size.y);
					  } break;
		case UI_LAYOUT_KIND_HORZ: {
						  layout->size.x = fmaxf(layout->size.x, size.x);
						  layout->size.y += size.y;
					  } break;
		case UI_LAYOUT_KIND_COUNT: 
		default: {
				 UI_ASSERT(0, "UNREACHABLE!");
			 } break;
	}
}

// Methods of UI_Context
UI_Context UI_Context_make() {
	UI_Context ctx = {0};
	ctx.active_id = -1;
	ctx.layouts_count = 0;

	return ctx;
}

#endif
