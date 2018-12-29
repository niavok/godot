/*************************************************************************/
/*  skeleton_ik_2d_editor_plugin.cpp                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2018 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2018 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "skeleton_ik_2d_editor_plugin.h"

#include "scene/animation/skeleton_ik_2d.h"

void SkeletonIK2DEditorPlugin::_play() {

	printf("play\n");
	if (!skeleton_ik)
		return;

	if (!skeleton_ik->get_parent_skeleton())
		return;

	if (play_btn->is_pressed()) {

		initial_bone_poses.resize(skeleton_ik->get_parent_skeleton()->get_bone_count());
		for (int i = 0; i < skeleton_ik->get_parent_skeleton()->get_bone_count(); ++i) {
			initial_bone_poses.write[i] = skeleton_ik->get_parent_skeleton()->get_bone_pose(i);
		}

		skeleton_ik->start();
	} else {
		skeleton_ik->stop();

		if (initial_bone_poses.size() != skeleton_ik->get_parent_skeleton()->get_bone_count())
			return;

		for (int i = 0; i < skeleton_ik->get_parent_skeleton()->get_bone_count(); ++i) {
			skeleton_ik->get_parent_skeleton()->set_bone_pose(i, initial_bone_poses[i]);
		}
	}
}

void SkeletonIK2DEditorPlugin::edit(Object *p_object) {
	printf("edit\n");
	if (p_object != skeleton_ik) {
		if (skeleton_ik) {
			play_btn->set_pressed(false);
			if(!multiple_play_checkbox->is_pressed())
			{
				_play();
			}
		}
	}

	SkeletonIK2D *s = Object::cast_to<SkeletonIK2D>(p_object);
	if (!s)
		return;

	skeleton_ik = s;
	printf("   edit set_pressed is runing\n");
	play_btn->set_pressed(skeleton_ik->is_running());
}

bool SkeletonIK2DEditorPlugin::handles(Object *p_object) const {

	return p_object->is_class("SkeletonIK2D");
}

int count_skeleton_ik_nodes(Node* node)
{
	int count = node->is_class("SkeletonIK2D") ? 1 : 0;

	int children_count = node->get_child_count();
	for(int i = 0; i < children_count; i++)
	{
		count += count_skeleton_ik_nodes(node->get_child(i));
	}
	return count;
}

void SkeletonIK2DEditorPlugin::make_visible(bool p_visible) {

	if (p_visible)
	{
		play_btn->show();

		int ik_count = count_skeleton_ik_nodes(get_editor_interface()->get_edited_scene_root());
		if(ik_count > 1)
			multiple_play_checkbox->show();
	}
	else
	{
		play_btn->hide();
		multiple_play_checkbox->hide();
	}
}

void SkeletonIK2DEditorPlugin::_bind_methods() {

	ClassDB::bind_method("_play", &SkeletonIK2DEditorPlugin::_play);
}

SkeletonIK2DEditorPlugin::SkeletonIK2DEditorPlugin(EditorNode *p_node) {

	editor = p_node;
	play_btn = memnew(Button);
	play_btn->set_icon(editor->get_gui_base()->get_icon("Play", "EditorIcons"));
	play_btn->set_text(TTR("Play IK"));
	play_btn->set_toggle_mode(true);
	play_btn->hide();
	play_btn->connect("pressed", this, "_play");

	multiple_play_checkbox = memnew(CheckBox);
	multiple_play_checkbox->set_text(TTR("Allow multiple IK"));
	multiple_play_checkbox->hide();

	add_control_to_container(CONTAINER_CANVAS_EDITOR_MENU, play_btn);
	add_control_to_container(CONTAINER_CANVAS_EDITOR_MENU, multiple_play_checkbox);
	skeleton_ik = NULL;
}

SkeletonIK2DEditorPlugin::~SkeletonIK2DEditorPlugin() {}
