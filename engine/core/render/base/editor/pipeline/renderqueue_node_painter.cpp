#include "renderqueue_node_painter.h"
#include "engine/core/main/Engine.h"

#ifdef ECHO_EDITOR_MODE

namespace Pipeline
{
	RenderQueueNodePainter::RenderQueueNodePainter(QGraphicsView* view, QGraphicsScene* scene, Echo::IRenderQueue* queue)
	{
		m_renderQueue = queue;
		m_graphicsView = view;
		m_graphicsScene = scene;

		if (!m_rect)
		{
			float halfWidth = m_width * 0.5f;
			float halfHeight = m_height * 0.5f;

			QPainterPath path;
			path.addRoundedRect(QRectF(-halfWidth, -halfHeight, m_width, m_height), m_style.m_cornerRadius, m_style.m_cornerRadius);

			m_rect = new QGraphicsRenderQueueItem(nullptr);
			m_rect->setPath(path);
			m_rect->setPen(QPen(m_style.m_normalBoundaryColor, m_style.m_penWidth));
			m_rect->setFlag(QGraphicsItem::ItemIsFocusable);
			m_rect->setAcceptHoverEvents(true);
			QLinearGradient gradient(QPointF(0.0, -halfHeight), QPointF(0.0, halfHeight));
			gradient.setColorAt(0.0, m_style.m_gradientColor0);
			gradient.setColorAt(0.03, m_style.m_gradientColor1);
			gradient.setColorAt(0.97, m_style.m_gradientColor2);
			gradient.setColorAt(1.0, m_style.m_gradientColor3);
			m_rect->setBrush(gradient);
			m_rect->setPos(QPointF(0.f, 0.f));
			m_graphicsScene->addItem(m_rect);

			// mouse press event
			m_rect->setMousePressEventCb([this](QGraphicsItem* item)
			{
				EditorApi.showObjectProperty(m_renderQueue);
			});

			m_rect->setKeyPressEventCb([this](QKeyEvent* event) 
			{
				if (m_renderQueue)
				{
					EditorApi.showObjectProperty(m_renderQueue->getStage());
					m_renderQueue->getStage()->deleteRenderQueue(m_renderQueue);
				}
			});

			m_text = m_graphicsScene->addSimpleText(m_renderQueue->getName().c_str());
			m_text->setBrush(QBrush(m_style.m_fontColor));
			m_text->setParentItem(m_rect);

			// close
			initDeleteButton();
		}
	}

	void RenderQueueNodePainter::initDeleteButton()
	{
		QPixmap icon((Echo::Engine::instance()->getRootPath() + "engine/core/render/base/editor/icon/delete.png").c_str());
		m_deleteButtton = new QGraphicsPixmapItemCustom();
		m_deleteButtton->setPixmap(icon.scaled(QSize(16, 16)));
		m_deleteButtton->setParentItem(m_rect);
		m_deleteButtton->setPos(QPointF(m_width * 0.5f - 24.f, -8.f));
		m_deleteButtton->setAcceptHoverEvents(true);
		m_deleteButtton->setVisible(false);
		m_graphicsScene->addItem(m_deleteButtton);

		m_rect->setHoverEnterEventCb([this](QGraphicsItem* item)
		{
			m_deleteButtton->setVisible(true);
		});

		m_rect->setHoverEnterLeaveCb([this](QGraphicsItem* item)
		{
			m_deleteButtton->setVisible(false);
		});

		m_deleteButtton->setMousePressEventCb([this](QGraphicsPixmapItem* item)
		{
			if (m_renderQueue)
			{
				EditorApi.showObjectProperty(m_renderQueue->getStage());
				m_renderQueue->getStage()->deleteRenderQueue(m_renderQueue);
			}
		});
	}

	void RenderQueueNodePainter::reset()
	{
		if (m_rect)
			m_graphicsScene->removeItem(m_rect);

		m_renderQueue = nullptr;
		m_graphicsView = nullptr;
		m_graphicsScene = nullptr;
		m_rect = nullptr;
		m_text = nullptr;
	}

	void RenderQueueNodePainter::update(Echo::i32 xPos, Echo::i32 yPos)
	{
		float halfWidth = m_width * 0.5f;
		float halfHeight = m_height * 0.5f;

		float startYPos = 60.f;
		m_rect->setPen(QPen(m_rect->isFocused() ? m_style.m_selectedBoundaryColor : m_style.m_normalBoundaryColor, m_style.m_penWidth));
		m_rect->setPos(xPos * 200.f, startYPos + yPos * 56.f);

		m_text->setText(m_renderQueue->getName().c_str());

		Echo::Rect textRect;
		EditorApi.qGraphicsItemSceneRect(m_text, textRect);
		m_text->setPos((m_width - textRect.getWidth()) * 0.5f - halfWidth, (m_height - textRect.getHeight()) * 0.5f - halfHeight);
	}
}

#endif