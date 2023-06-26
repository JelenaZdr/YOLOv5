
#pragma once

#include <QString>
#include <QVector>
#include <QImage>

void imageProcessingFun(
	const QString& prog_name,
	QImage& out_img,
	const QImage& in_img,
	const QVector<double>& params
);
