/*
 * Mana Mobile
 * Copyright 2010 Thorbjørn Lindeijer
 */

#ifndef SHA256_H
#define SHA256_H

#include <QByteArray>

/**
 * Returns the SHA-256 hash for the given data.
 *
 * @param data the data to create the SHA-256 hash for
 * @return the SHA-256 hash for the given data.
 */
QByteArray sha256(const QByteArray &data);

#endif // SHA256_H
