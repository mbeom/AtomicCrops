#include "stdafx.h"
#include "Item.h"
#include "player.h"

HRESULT Item::init()
{
	return S_OK;
}

HRESULT Item::init(float x, float y, int num)
{
	return S_OK;
}

HRESULT Item::init(float x, float y, bool is_taken)
{
	return S_OK;
}

void Item::release()
{
}

void Item::update()
{
	if (object != NULL && is_selling == false)
	{
		count += TIMEMANAGER->getElapsedTime();
		if (count > 1.2f) count = 0;

		object->gravity += object->gravity * TIMEMANAGER->getElapsedTime() * 10;
		if (object->gravity > 5.0) object->is_drop = true;

		if (object->is_drop)
		{
			if (count > 1.0f && count < 1.2f)
			{
				object->bounce += 0.5 * TIMEMANAGER->getElapsedTime();
				object->y += 1.0f * -sinf(object->bounce * 180 / PI);
				object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
			}
		}

		if (!object->is_taken && !object->is_drop)
		{
			object->x += object->speed * cosf(object->angle);
			object->y += object->speed * -sinf(object->angle) + 5 * object->gravity;
			object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
		}

		if (object->is_taken)
		{
			object->x += object->speed * cosf(object->angle);
			object->y += object->speed * -sinf(object->angle);
			object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
		}
	}

	if (is_selling && is_closed)
	{
		if (!is_ready)
			loading += TIMEMANAGER->getElapsedTime();

		if (loading > 0.1f)
			is_ready = true;

		int font_size = 20;

		bottom_title.x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
		bottom_title.y = CAMERAMANAGER->screen.bottom * 0.8 + bottom_title.img->getHeight();
		bottom_title.rc = D2DMANAGER->RectMakeCenter(bottom_title.x, bottom_title.y,
			bottom_title.description.length() * font_size * 1.2, font_size * 1.2);

		bottom_rect.x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
		bottom_rect.y = CAMERAMANAGER->screen.bottom * 0.85;
		bottom_rect.rc = D2DMANAGER->RectMakeCenter(bottom_rect.x, bottom_rect.y,
			WINSIZEX * 0.25, WINSIZEY * 0.1);

		bottom_description.x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
		bottom_description.y = CAMERAMANAGER->screen.bottom * 0.845;
		bottom_description.rc = D2DMANAGER->RectMakeCenter(bottom_description.x, bottom_description.y,
			WINSIZEX * 0.245, WINSIZEY * 0.075);

		top_box.x = (object->rc.left + object->rc.right) / 2;
		top_box.y = object->rc.top * 0.95;
		top_box.rc = D2DMANAGER->RectMakeCenter(top_box.x, top_box.y,
			(bottom_title.description.length() + 2) * font_size * 1.1, font_size * 1.3);

		top_description.x = (object->rc.left + object->rc.right) / 2;
		top_description.y = object->rc.top * 0.95 - top_box.height * 0.5;
		top_description.rc = D2DMANAGER->RectMake(top_description.x, top_description.y,
			top_box.width * 0.5, top_box.height);
	}

}

void Item::render()
{
	if (type != ItemType::FERTILIZER)
	{
		if (_img)
			_img->render(rc.left, rc.top);

		if (object != NULL)
			object->img->render(object->rc.left, object->rc.top);

		if (is_closed && is_ready)
		{
			bottom_rect.img->render(bottom_rect.rc.left, bottom_rect.rc.top, 0.7f);
			bottom_description.img->render(bottom_description.rc.left, bottom_description.rc.top);
			bottom_title.img->render(bottom_title.rc.left, bottom_title.rc.top, 0.7f);

			top_box.img->render(top_box.rc.left, top_box.rc.top, 0.9f);
			top_description.img->render(top_description.rc.left + 15, top_description.rc.top + 5);

			D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 20, bottom_title.description.c_str(),
				bottom_title.rc.left, bottom_title.rc.top, bottom_title.rc.right, bottom_title.rc.bottom);

			D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 20, bottom_description.description.c_str(),
				bottom_description.rc.left, bottom_description.y - 10, bottom_description.rc.right, bottom_description.rc.bottom);

			D2DMANAGER->DrawTextD2D(D2DMANAGER->white_brush, L"메이플스토리", 20, top_box.description.c_str(),
				top_box.rc.left + 5, top_box.y - 10, top_box.rc.right, top_box.rc.bottom);

			D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 16, top_description.description.c_str(),
				top_description.rc.left + 25, top_description.rc.top + 5, top_description.rc.right, top_description.rc.bottom);
		}

		//D2DMANAGER->Rectangle(object->rc);
	}
	else
	{
		if (object != NULL)
			object->img->aniRender(object->rc.left, object->rc.top, _ani);
	}

}
