#include "ui_page.h"

#ifdef TFT_IN_USE

#include "ui_button.h"
#include "utils_page.h"

void ui_page::set_page_id(uint8_t id)
{
  page_id = id;
}

void ui_page::set_page_name(char *name)
{
  page_name[0] = '\0';
  if (name)
  {
    strcpy(page_name, name);
    page_name[MAX_TEXT_SIZE - 1] = '\0';
  }
}

void ui_page::draw_work_area(void)
{
  uint8_t i = 0;
  for (i = 0; i < work_area_obj_cnt[active_work_area]; i++)
    obj_list_work_area[active_work_area][i]->draw();
  
  draw_work_area_arrows();
}

void ui_page::draw(void)
{
  toolbar->draw();

  draw_work_area();
}

void ui_page::clear_work_area(void)
{
	tft_clear_work_area();
}

void ui_page::draw_arrow(work_area_arrow_type_t type, uint16_t color)
{
  int16_t x0 = 0, y0 = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  if (type == ARROW_LEFT)
  {
    x0 = WORK_AREA_ARROW_LEFT_X0;
    y0 = WORK_AREA_ARROW_LEFT_Y0;
    x1 = WORK_AREA_ARROW_LEFT_X1;
    y1 = WORK_AREA_ARROW_LEFT_Y1;
    x2 = WORK_AREA_ARROW_LEFT_X2;
    y2 = WORK_AREA_ARROW_LEFT_Y2;
  }
  else if (type == ARROW_RIGHT)
  {
    x0 = WORK_AREA_ARROW_RIGHT_X0;
    y0 = WORK_AREA_ARROW_RIGHT_Y0;
    x1 = WORK_AREA_ARROW_RIGHT_X1;
    y1 = WORK_AREA_ARROW_RIGHT_Y1;
    x2 = WORK_AREA_ARROW_RIGHT_X2;
    y2 = WORK_AREA_ARROW_RIGHT_Y2;
  }
  else if (type == ARROW_UP)
  {
    x0 = WORK_AREA_ARROW_UP_X0;
    y0 = WORK_AREA_ARROW_UP_Y0;
    x1 = WORK_AREA_ARROW_UP_X1;
    y1 = WORK_AREA_ARROW_UP_Y1;
    x2 = WORK_AREA_ARROW_UP_X2;
    y2 = WORK_AREA_ARROW_UP_Y2;
  }
  else if (type == ARROW_DOWN)
  {
    x0 = WORK_AREA_ARROW_DOWN_X0;
    y0 = WORK_AREA_ARROW_DOWN_Y0;
    x1 = WORK_AREA_ARROW_DOWN_X1;
    y1 = WORK_AREA_ARROW_DOWN_Y1;
    x2 = WORK_AREA_ARROW_DOWN_X2;
    y2 = WORK_AREA_ARROW_DOWN_Y2;
  }

  tft_fill_traingle(x0, y0, x1, y1, x2, y2, color);
}

void ui_page::draw_work_area_arrows(void)
{
  if (free_work_area > 0)
  {
    if ((work_area_type == WORK_GRID) || (work_area_type == WORK_FREE))
    {
      if (active_work_area == 0)
        draw_arrow(ARROW_RIGHT, COLOR_YELLOW);
      else if (active_work_area == (WORK_AREA_PER_PAGE - 1))
        draw_arrow(ARROW_LEFT, COLOR_YELLOW);
      else
      {
        draw_arrow(ARROW_RIGHT, COLOR_YELLOW);
        draw_arrow(ARROW_LEFT, COLOR_YELLOW);
      }
    }
    else
    {
      if (active_work_area == 0)
        draw_arrow(ARROW_DOWN, COLOR_YELLOW);
      else if (active_work_area == (WORK_AREA_PER_PAGE - 1))
        draw_arrow(ARROW_UP, COLOR_YELLOW);
      else
      {
        draw_arrow(ARROW_DOWN, COLOR_YELLOW);
        draw_arrow(ARROW_UP, COLOR_YELLOW);
      }  
    }
  }
}

bool ui_page::add_toolbar_obj(ui_obj *obj)
{
  if (obj_cnt == MAX_OBJ_PER_PAGE)
    return false;

  if (toolbar->add_obj(obj))
  {
    obj_cnt++;

    return true;
  }

  return false;
}

void ui_page::set_work_area_type(work_area_type_t type)
{
  work_area_type = type;
}

bool ui_page::add_work_area_obj(ui_obj *obj)
{
  if (obj_cnt == MAX_OBJ_PER_PAGE)
    return false;

  if (work_area_type == WORK_GRID)
  {
    if (work_area_obj_cnt[free_work_area] == WORK_AREA_OBJ_MAX)
    {
      if (free_work_area == (WORK_AREA_PER_PAGE - 1))
        return false;
      free_work_area++;
    }

    obj->x = WORK_AREA_FRAME_GAP + ((WORK_AREA_OBJ_SIZE + WORK_AREA_OBJ_GAP_X_GRID) * (work_area_obj_cnt[free_work_area] % WORK_AREA_OBJ_COL)) + WORK_AREA_OBJ_GAP_X_GRID;
    obj->y = WORK_AREA_FRAME_Y + ((WORK_AREA_OBJ_SIZE + WORK_AREA_OBJ_GAP_Y_GRID) * (work_area_obj_cnt[free_work_area] / WORK_AREA_OBJ_COL)) + WORK_AREA_OBJ_GAP_Y_GRID;
    obj->h = WORK_AREA_OBJ_SIZE;
    obj->w = WORK_AREA_OBJ_SIZE;
  }
  else if (work_area_type == WORK_ROWS)
  {
    if (work_area_obj_cnt[free_work_area] == WORK_AREA_OBJ_ROW)
    {
      if (free_work_area == (WORK_AREA_PER_PAGE - 1))
        return false;
      free_work_area++;
    }

    obj->x = WORK_AREA_FRAME_GAP + WORK_AREA_OBJ_GAP_X_RAW;
    obj->y = WORK_AREA_FRAME_Y + ((WORK_AREA_OBJ_SIZE + WORK_AREA_OBJ_GAP_Y_GRID) * work_area_obj_cnt[free_work_area]) + WORK_AREA_OBJ_GAP_Y_GRID;
    obj->h = WORK_AREA_OBJ_SIZE;
    obj->w = WORK_AREA_OBJ_W_RAW;
  }
  else
  {
    if (work_area_obj_cnt[free_work_area] == WORK_AREA_OBJ_MAX)
    {
      if (free_work_area == (WORK_AREA_PER_PAGE - 1))
        return false;
      free_work_area++;
    }
    // Do nothing. Use x,y of the object
    // TODO : check for overlap !!!
  }

  obj->set_text_xy();

  obj_cnt++;

  if (obj->obj_type != OBJ_NO_TOUCH)
  {
    obj_touch_list[free_work_area][obj_touch_cnt[free_work_area]++] = obj;
  }

  obj_list_work_area[free_work_area][work_area_obj_cnt[free_work_area]++] = obj;

  return true;
}

ui_obj *ui_page::find_touch(touch_event_t event)
{
  ui_obj *obj = NULL;

  do
  {
    if (active_obj)
    {
      if (active_obj->is_touched(event))
      {
        obj = active_obj;
        break;
      }
      else
      {
        active_obj->draw();
        active_obj = NULL;
      }
    }

    if ((obj = toolbar->find_touch(event)) != NULL)
      break;

    uint8_t i = 0;
    for (; i < obj_touch_cnt[active_work_area]; i++)
    {
      if (obj_touch_list[active_work_area][i]->is_touched(event))
      {
        obj = obj_touch_list[active_work_area][i];
        break;
      }
    }
  } while (0);

  if (obj)
  {
    if (event.type == T_DOWN)
      active_obj = obj;
  }

  return obj;
}

void ui_page::set_touch_handler(touch_func_t f)
{
  handle_touch_fn = f;
}

void ui_page::handle_touch(ui_page *page, touch_event_t event)
{
  if (handle_touch_fn)
    handle_touch_fn(page, event);
}

void onBackButtonUP(ui_page *page, ui_button *button, touch_event_t event)
{
  if (page->active_obj == (ui_obj *)button)
  {
    launch_prev_page();
  }
}

void onHomeButtonUP(ui_page *page, ui_button *button, touch_event_t event)
{
  if (page->active_obj == (ui_obj *)button)
  {
    launch_page(ID_HOME);
  }
}

void ui_page::handle_flags(uint8_t flags)
{
  if (flags & TOOLBAR_FLAG_HOME)
  {
    ui_button *home_button = new ui_button(0, 0, 0, 0, ID_HOME_BUTTON);
    home_button->set_bmp(HOME_UP_BMP, T_UP);
    home_button->set_bmp(HOME_DOWN_BMP, T_DOWN);
    home_button->set_on_touch_handler(T_UP, onHomeButtonUP);
    add_toolbar_obj(home_button);
  }

  if (flags & TOOLBAR_FLAG_BACK)
  {
    ui_button *back_button = new ui_button(0, 0, 0, 0, ID_BACK_BUTTON);
    back_button->set_bmp(BACK_UP_BMP, T_UP);
    back_button->set_bmp(BACK_DOWN_BMP, T_DOWN);
    back_button->set_on_touch_handler(T_UP, onBackButtonUP);
    add_toolbar_obj(back_button);
  }
}

bool default_swipe_handler(ui_page *page, touch_event_t event)
{
  if (event.type > T_DOWN)
  {
    bool draw = false;
    if ((page->work_area_type == WORK_GRID) || (page->work_area_type == WORK_FREE))
    {
      if (event.type == T_SWIPE_LEFT)
      {
        if (page->active_work_area < (WORK_AREA_PER_PAGE - 1))
        {
          page->active_work_area++;
          draw = true;
        }
      }
      else if (event.type == T_SWIPE_RIGHT)
      {
        if (page->active_work_area > 0)
        {
          page->active_work_area--;
          draw = true;
        }
      }
    }
    else
    {
      if (event.type == T_SWIPE_UP)
      {
        if (page->active_work_area < (WORK_AREA_PER_PAGE - 1))
        {
          page->active_work_area++;
          draw = true;
        }
      }
      else if (event.type == T_SWIPE_DOWN)
      {
        if (page->active_work_area > 0)
        {
          page->active_work_area--;
          draw = true;
        }
      }
    }

    if (draw)
    {
      page->clear_work_area();
      page->draw_work_area();
      return true;
    }
  }

  return false;
}

void default_touch_handler(ui_page *page, touch_event_t event) // Assumes objects are buttons
{
  if (default_swipe_handler(page, event))
    return;

  ui_button *b = (ui_button *)page->find_touch(event);
  if (b)
    b->on_touch(page, b, event);
}

#endif // TFT_IN_USE
