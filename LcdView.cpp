/****************************************************************************
 LcdView.cpp
****************************************************************************/
#include <QDebug>
#include <QGridLayout>
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>

#include "LcdView.h"

// Class : LcdView Class

LcdView::LcdView( void ) 
{
	bool ret = true;
	qDebug() << "Constructor is called!";

	ret = initLayout();
	if( false == ret ){
		qDebug() << "initLayout is failed !";
		return;
	}
	ret = initUdpSocket();
	if( false == ret ){
		qDebug() << "initBinfile is failed !";
		return;
	}
}

bool LcdView::initLayout(void)
{
	bool ret = true;
	
	qDebug() << "Initialized key view layout.";

#if 0
	QGridLayout	*mainLayout = new QGridLayout( this );
	m_UpBtn		= new QPushButton( "[UP]", this );
	m_LeftBtn	= new QPushButton( "[<L]", this );
	m_RightBtn	= new QPushButton( "[R>]", this );
	m_DownBtn	= new QPushButton( "[DOWN]", this );
	QHBoxLayout	*lowerLayout = new QHBoxLayout( this );
	m_EnterBtn	= new QPushButton( "ENT", this );
	m_ClearBtn	= new QPushButton( "CLR", this );

	// Layout keys.
					//	row		column		height		width
	mainLayout->addWidget( m_UpBtn,		0, 		1, 		1, 		2 );
	mainLayout->addWidget( m_LeftBtn,	1, 		0, 		1, 		2 );
	mainLayout->addWidget( m_RightBtn,	1, 		2, 		1, 		2 );
	mainLayout->addWidget( m_DownBtn,	2, 		1, 		1, 		2 );

	lowerLayout->addStretch();
	lowerLayout->addWidget( m_EnterBtn );
	lowerLayout->addWidget( m_ClearBtn );
	mainLayout->addLayout( lowerLayout,	3,		0, 		1, 		4 );

	// Regist key clicked event.
	connect( m_UpBtn,		SIGNAL( pressed() ), this, SLOT( upPressed() ) );
	connect( m_DownBtn,		SIGNAL( pressed() ), this, SLOT( downPressed() ) );
	connect( m_LeftBtn,		SIGNAL( pressed() ), this, SLOT( leftPressed() ) );
	connect( m_RightBtn,	SIGNAL( pressed() ), this, SLOT( rightPressed() ) );
	connect( m_EnterBtn,	SIGNAL( pressed() ), this, SLOT( enterPressed() ) );
	connect( m_ClearBtn,	SIGNAL( pressed() ), this, SLOT( clearPressed() ) );

	// Regist key clicked event.
	connect( m_UpBtn,		SIGNAL( released() ), this, SLOT( upReleased() ) );
	connect( m_DownBtn,		SIGNAL( released() ), this, SLOT( downReleased() ) );
	connect( m_LeftBtn,		SIGNAL( released() ), this, SLOT( leftReleased() ) );
	connect( m_RightBtn,	SIGNAL( released() ), this, SLOT( rightReleased() ) );
	connect( m_EnterBtn,	SIGNAL( released() ), this, SLOT( enterReleased() ) );
	connect( m_ClearBtn,	SIGNAL( released() ), this, SLOT( clearReleased() ) );

	// Set layout to dialog( e.q LcdView ).
	setLayout( mainLayout );
#endif
	return ret;
}

bool LcdView::initUdpSocket(void)
{
	bool ret = true;

	m_udpSocket = new QUdpSocket( this );
	if( NULL == m_udpSocket ){
		qDebug() << "Failed to initialized udp socket!" << endl;
		return (false);
	}
	m_udpSocket->bind( QHostAddress::LocalHost, LCDVIEW_CMD_UDP_PORT );
	connect( m_udpSocket, SIGNAL( readyRead() ), this, SLOT( readPendingDatagrams() ) );

	return ret;
}

void LcdView::readPendingDatagrams(void)
{
	if( NULL == m_udpSocket ){
		qDebug() << "Udp socket is not exist!" << endl;
		return;
	}

	QByteArray	datagram;

	while( m_udpSocket->hasPendingDatagrams() ){
		datagram.resize( m_udpSocket->pendingDatagramSize() );
		QHostAddress	sender;
		quint16		senderPort;

		m_udpSocket->readDatagram( 
				datagram.data(), 
				datagram.size(),
		                &sender,
				&senderPort);

	}


	qint32		cmdLen;		// コマンドの長さ
	qint8		code;		// 制御コード
	QString		lcdL1Str;	// LCD1行目表示文字列
	QString		lcdL2Str;	// LCD2行目表示文字列

	cmdLen = datagram.length();
	if( 2 > cmdLen ){
		qDebug() << "Recieved data is too short !" << endl;
	}
	QDataStream	in( &datagram, QIODevice::ReadOnly );
	in >> code;
	in >> lcdL1Str;

//	switch( code ){
//	case '8' :
//	case '9' :
//	}
	
	qDebug() << lcdL1Str;
}
