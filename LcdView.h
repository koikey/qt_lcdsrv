/****************************************************************************
****************************************************************************/
#ifndef LCD_VIEW_HEADER
#define LCD_VIEW_HEADER

#include <QDialog>
#include <QUdpSocket>
#include <QDebug>

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */
#ifdef __cplusplus
}
#endif /* __cplusplus */


#define LCDVIEW_CMD_UDP_PORT	(5001)

// Class : LcdView Class
// Input lcd view.
class LcdView : public QDialog
{
	Q_OBJECT

public:
	LcdView( void );
	
private:
	QPushButton	*m_ClearBtn;	// [CLR] Button
	QUdpSocket	*m_udpSocket;

	// Initalized keyview layout.
	bool initLayout( void );
	bool initUdpSocket( void );

public slots:
	// Udp data recieved slot.
	void readPendingDatagrams( void );


};

#endif /* LCD_VIEW_HEADER */
