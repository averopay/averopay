#include "marketbrowser.h"
#include "ui_marketbrowser.h"
#include "main.h"
#include "wallet.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include <QDesktopServices>

#include <sstream>
#include <string>

using namespace json_spirit;

const QString kBaseUrl = "https://averopay.com/aopusd.php";
const QString kBaseUrl1 = "https://www.blockchain.com/tobtc?currency=USD&value=1";
const QString kBaseUrl2 = "https://averopay.com/aopmc.php";
const QString kBaseUrl3 = "https://averopay.com/aopbtc.php";

QString bitcoinp = "";
QString AveroPayp = "";
QString AOPmcp = "";
QString AOPbtcp = "";
double bitcoin2;
double AveroPay2;
double AOPmc2;
double AOPbtc2;
QString bitcoing;
QString AOPmarket;
QString dollarg;
int mode=1;
int o = 0;


MarketBrowser::MarketBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketBrowser)
{
    ui->setupUi(this);
    setFixedSize(400, 420);


requests();
QObject::connect(&m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponse(QNetworkReply*)));
connect(ui->startButton, SIGNAL(pressed()), this, SLOT( requests()));
connect(ui->egal, SIGNAL(pressed()), this, SLOT( update()));

}

void MarketBrowser::update()
{
    QString temps = ui->egals->text();
    double totald = dollarg.toDouble() * temps.toDouble();
    double totaldq = bitcoing.toDouble() * temps.toDouble();
    ui->egald->setText("$ "+QString::number(totald)+" USD or "+QString::number(totaldq)+" BTC");

}

void MarketBrowser::requests()
{
	getRequest(kBaseUrl);
    getRequest(kBaseUrl1);
	getRequest(kBaseUrl2);
	getRequest(kBaseUrl3);
}

void MarketBrowser::getRequest( const QString &urlString )
{
    QUrl url ( urlString );
    QNetworkRequest req ( url );
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    m_nam.get(req);
}

void MarketBrowser::parseNetworkResponse(QNetworkReply *finished )
{

    QUrl what = finished->url();

    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        emit networkError( finished->error() );
        return;
    }
	
if (what == kBaseUrl) // AveroPay Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString AveroPay = finished->readAll();
    AveroPay2 = (AveroPay.toDouble());
    AveroPay = QString::number(AveroPay2, 'f', 2);
	
    if(AveroPay > AveroPayp)
    {
        ui->AveroPay->setText("<font color=\"yellow\">$" + AveroPay + "</font>");
    } else if (AveroPay < AveroPayp) {
        ui->AveroPay->setText("<font color=\"red\">$" + AveroPay + "</font>");
        } else {
    ui->AveroPay->setText("$"+AveroPay+" USD");
    }

    AveroPayp = AveroPay;
	dollarg = AveroPay;
}

if (what == kBaseUrl1) // Bitcoin Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString bitcoin = finished->readAll();
    bitcoin2 = (1 / bitcoin.toDouble());
    bitcoin = QString::number(bitcoin2, 'f', 2);
    if(bitcoin > bitcoinp)
    {
        ui->bitcoin->setText("<font color=\"yellow\">$" + bitcoin + " USD</font>");
    } else if (bitcoin < bitcoinp) {
        ui->bitcoin->setText("<font color=\"red\">$" + bitcoin + " USD</font>");
        } else {
    ui->bitcoin->setText("$"+bitcoin+" USD");
    }

    bitcoinp = bitcoin;
}

if (what == kBaseUrl2) // AveroPay Market Cap
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString AOPmc = finished->readAll();
    AOPmc2 = (AOPmc.toDouble());
    AOPmc = QString::number(AOPmc2, 'f', 2);
	
    if(AOPmc > AOPmcp)
    {
        ui->AOPmc->setText("<font color=\"yellow\">$" + AOPmc + "</font>");
    } else if (AOPmc < AOPmcp) {
        ui->AOPmc->setText("<font color=\"red\">$" + AOPmc + "</font>");
        } else {
    ui->AOPmc->setText("$"+AOPmc+" USD");
    }

    AOPmcp = AOPmc;
	AOPmarket = AOPmc;
}

if (what == kBaseUrl3) // AveroPay BTC Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString AOPbtc = finished->readAll();
    AOPbtc2 = (AOPbtc.toDouble());
    AOPbtc = QString::number(AOPbtc2, 'f', 8);
	
    if(AOPbtc > AOPbtcp)
    {
        ui->AOPbtc->setText("<font color=\"yellow\">" + AOPbtc + " BTC</font>");
    } else if (AOPbtc < AOPbtcp) {
        ui->AOPbtc->setText("<font color=\"red\">" + AOPbtc + " BTC</font>");
        } else {
    ui->AOPbtc->setText(AOPbtc+" BTC");
    }

    AOPbtcp = AOPbtc;
	bitcoing = AOPbtc;
}

finished->deleteLater();
}


void MarketBrowser::setModel(ClientModel *model)
{
    this->model = model;
}

MarketBrowser::~MarketBrowser()
{
    delete ui;
}
