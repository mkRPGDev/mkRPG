#ifndef BCOLOR_H
#define BCOLOR_H

#include <QtWidgets>


/*!
 * \file bcolor.h
 *
 * \brief Definition of the BColor class.
 *
 *
 * \author Baptiste Pauget
 */


// TODO hasOpacity, isReadOnly

/*!
 * \brief The BColor class is a simple frame that offers color selection.
 *
 *
 */
class BColor : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged) /**<
     * The current color that is displayed by the widget
     *
     * Default value : QColor(0,0,0)
     *
     * \see \ref setColor,  \ref setColorQuiet, \ref colorChanged.
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged) /**<
     * The name that is shown as title for the color chooser dialog
     * used for user color definition purpose.
     *
     * \see \ref setName, \ref setNameQuiet, and \ref nameChanged.
     */
public:
    explicit BColor(QWidget *parent = 0); /**<
     * Constructs a new BColor object, with white as current color.
     */
    explicit BColor(QColor c, QWidget *parent = 0); /**<
     * Constructs a new BColor object and sets the color to c.
     */
    const QString &name() const; /**<
     * Returns the name of the selector.
     *
     * \see \ref setName, \ref setNameQuiet, \ref nameChanged
     */
    const QColor &color() const; /**<
     * Returns the current color of the selector.
     *
     * \see \ref setColor, \ref setColorQuiet, \ref colorChanged
     */

signals:
    void colorChanged(const QColor&); /**<
     * This signal is emitted when the color change, both when
     * the user edit it or when \ref setColor is called.
     *
     * \see \ref color, \ref setColorQuiet
     */
    void nameChanged(const QString&); /**<
     * This signal is emitted when the name change, when
     * \ref setColor is called.
     *
     * \see \ref name, \ref setNameQuiet
     */

public slots:
    void setColor(const QColor &c); /**<
     * Sets the current color.
     *
     * The signal \ref colorChanged is emitted.
     *
     * \see \ref setColorQuiet, \ref color
     */
    void setColorQuiet(const QColor &c); /**<
     * Sets the current color.
     *
     * The signal \ref colorChanged is not emitted.
     *
     * \see \ref setColor, \ref color
     */
    void setName(const QString &s); /**<
     * Sets the name of the selector.
     *
     * The signal \ref nameChanged is emitted.
     *
     * \see \ref setNameQuiet, \ref name
     */
    void setNameQuiet(const QString &s); /**<
     * Sets the name of the selector.
     *
     * The signal \ref nameChanged is not emitted.
     *
     * \see \ref setName, \ref name
     */

private:
    void mousePressEvent(QMouseEvent *me);
    QColor coul;
    QString n;
};

#endif // BCOLOR_H
