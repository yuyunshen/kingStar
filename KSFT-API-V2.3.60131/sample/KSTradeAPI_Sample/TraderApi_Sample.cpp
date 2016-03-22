//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///@system ÆÚ»õ½»Ò×²âÊÔÊ¾Àý
///@company SunGard China
///@fileTraderApi_Sample
///@brief ½»Ò×Api Sample
///@history 
///20140801	Kingstar IRDG		kevin ´´½¨¸ÃÎÄ¼þ
///ÔËÐÐ´Ë³ÌÐòÇ°ÇëÈ·±£ IP¶Ë¿Ú BrokerID ÓÃ»§ÃûÃÜÂë¼°ÊÚÈ¨ÎÄ¼þÕýÈ·
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "KSTradeAPI.h"
#include "../../sdk/IncEx/KSUserApiStructEx.h"
#include <iostream>
#ifdef WIN32
#include "windows.h"
#endif

using namespace std;
//Ê¹ÓÃKingstarAPIÃüÃû¿Õ¼ä
using namespace KingstarAPI;

class CTraderApiSample : public CThostFtdcTraderSpi ,public CKSVocSpi
{
public: 
	CTraderApiSample(CThostFtdcTraderApi *pUserApi, TThostFtdcBrokerIDType chBrokerID, TThostFtdcUserIDType chUserID, TThostFtdcPasswordType chPassword, TThostFtdcContractCodeType chContract)
	{
		m_nRequestID = 0;
		m_pUserApi = pUserApi;
		strncpy(m_chBrokerID, chBrokerID, sizeof(m_chBrokerID)-1);
		strncpy(m_chUserID, chUserID, sizeof(m_chUserID)-1);
		strncpy(m_chPassword, chPassword, sizeof(m_chPassword)-1);
		strncpy(m_chContract, chContract, sizeof(m_chContract)-1);
	}


	~CTraderApiSample() {}

	// After making a succeed connection with the server, the client should send the login request to the server.
	virtual void OnFrontConnected()
	{
		printf("OnFrontConnected:\n");

		CThostFtdcReqUserLoginField reqUserLogin;
		memset(&reqUserLogin, 0, sizeof(reqUserLogin));
		// set BrokerID
		printf("BrokerID:%s\n", m_chBrokerID);
		strcpy(reqUserLogin. BrokerID, m_chBrokerID);

		// set user id
		printf("userid:%s\n", m_chUserID);
		strcpy(reqUserLogin.UserID, m_chUserID);

		// set password
		printf("password:%s\n",m_chPassword);
		strcpy(reqUserLogin.Password, m_chPassword);

		// send the login request
		m_pUserApi->ReqUserLogin(&reqUserLogin, m_nRequestID++ );
	}

	//When the connection between client and the server disconnected,the follwing function will be called.
	virtual void OnFrontDisconnected(int nReason)
	{ 
		//  In this  case,  API  will reconnect£¬the client application can ignore this.
		printf("OnFrontDisconnected.\n");
	} 

	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
	{
		printf("OnRtnInstrumentStatus:");
		if (NULL != pInstrumentStatus)
		{
			printf("%s-%c-%c",pInstrumentStatus->ExchangeID, pInstrumentStatus->InstrumentStatus, pInstrumentStatus->EnterReason);
		}
		printf("\n");
	}

	///±¨µ¥Â¼Èë´íÎó»Ø±¨
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
	{
		printf("OnErrRtnOrderInsert:\n");

	}

	///±¨µ¥²Ù×÷´íÎó»Ø±¨
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
	{
		printf("OnErrRtnOrderAction:\n");
	}

	// After receiving the login request from the client£¬the server will send the following response to notify the client whether the login success or not.
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspUserLogin:");
		if (pRspUserLogin != NULL)
		{
			printf("%s|%s|%s|%s|%s|%s|%s|%s|%s|%d|%d|%s|", 
				pRspUserLogin->BrokerID,					// ¾­¼Í¹«Ë¾´úÂë
				pRspUserLogin->UserID,						// ÓÃ»§´úÂë
				pRspUserLogin->TradingDay,					// ½»Ò×ÈÕ
				pRspUserLogin->SystemName,					// ½»Ò×ÏµÍ³Ãû³Æ
				pRspUserLogin->LoginTime,					// µÇÂ½³É¹¦Ê±¼ä
				pRspUserLogin->SHFETime,					// ÉÏÆÚËùÊ±¼ä
				pRspUserLogin->DCETime,						// ´óÉÌËùÊ±¼ä
				pRspUserLogin->CZCETime,					// Ö£ÉÌËùÊ±¼ä
				pRspUserLogin->FFEXTime,					// ÖÐ½ðËùÊ±¼ä
				pRspUserLogin->FrontID,						// frond id
				pRspUserLogin->SessionID,					// session id
				pRspUserLogin->MaxOrderRef					// ×î´ó±¨µ¥ÒýÓÃ
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
		if (pRspInfo->ErrorID != 0)
		{
			// in case any login failure, the client should handle this error.
			printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
			return;
		}

		// get trading day
		printf("%s\n",m_pUserApi->GetTradingDay());
		// qryInvestor request.
		CThostFtdcQryInvestorField Investor;
		memset(&Investor, 0, sizeof(Investor));
		// broker id 
		strcpy(Investor.BrokerID, m_chBrokerID);
		// investor ID 
		strcpy(Investor.InvestorID, m_chUserID);

		m_pUserApi->ReqQryInvestor(&Investor, m_nRequestID++ );
	}

	// investor response
	virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryInvestor:");
		if (NULL != pInvestor)
		{
			printf("%s|%s|%s|%s|%s|%s|%c|%s|%s|",
				pInvestor->InvestorID,						// Í¶×ÊÕß´úÂë
				pInvestor->InvestorName,					// Í¶×ÊÕßÃû³Æ
				pInvestor->IdentifiedCardNo,				// Ö¤¼þºÅÂë
				pInvestor->Telephone,						// ÁªÏµµç»°
				pInvestor->Address,							// Í¨Ñ¶µØÖ·
				pInvestor->InvestorGroupID,					// Í¶×ÊÕß·Ö×é´úÂë
				pInvestor->IdentifiedCardType,				// Ö¤¼þÀàÐÍ
				pInvestor->Mobile,							// ÊÖ»ú
				pInvestor->OpenDate);						// ¿ª»§ÈÕÆÚ
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg); 
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// tradeaccount request.
			CThostFtdcQryTradingAccountField TradingAccount;
			memset(&TradingAccount, 0, sizeof(TradingAccount));
			// broker id 
			strcpy(TradingAccount.BrokerID, m_chBrokerID);
			// investor ID 
			strcpy(TradingAccount.InvestorID, m_chUserID);

			m_pUserApi->ReqQryTradingAccount(&TradingAccount, m_nRequestID++ );
		}
	}

	// tradeaccount response
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryTradingAccount:");
		if (NULL != pTradingAccount)
		{
			printf(":%s|%.04f|%.04f|%.04f|%.04f|%.04f|%.04f|%.04f|%.04f|%.04f|%.04f|%.04f|",
				pTradingAccount->AccountID,					// ÕËºÅ
				pTradingAccount->PreBalance,				// ÉÏ´Î½áËã×¼±¸½ð
				pTradingAccount->Available,					// ¿ÉÓÃ×Ê½ð
				pTradingAccount->Commission,				// ÊÖÐø·Ñ
				pTradingAccount->PositionProfit,			// ³Ö²ÖÓ¯¿÷
				pTradingAccount->CloseProfit,				// Æ½²ÖÓ¯¿÷
				pTradingAccount->FrozenCommission,			// ¶³½áµÄÊÖÐø·Ñ
				pTradingAccount->FrozenCash,				// ¶³½áµÄ×Ê½ð
				pTradingAccount->CurrMargin,				// µ±Ç°±£Ö¤½ð×Ü¶î
				pTradingAccount->ExchangeMargin,			// ½»Ò×Ëù±£Ö¤½ð
				pTradingAccount->Mortgage,					// ÖÊÑº½ð¶î
				pTradingAccount->Credit);					// ÐÅÓÃ¶î¶È
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		// QryExchange
		CThostFtdcQryExchangeField QryExchange;
		memset(&QryExchange, 0, sizeof(QryExchange));
		// exchange id
		//½»Ò×Ëù´úÂë ÉÏÆÚSHFE  ´óÉÌDCE  Ö£ÉÌCZCE  ÖÐ½ðCFFEX  Ô­ÓÍINE  ÉÏÖ¤SSE  ÉîÖ¤SZSE
		strcpy(QryExchange.ExchangeID, "SHFE");

		m_pUserApi->ReqQryExchange(&QryExchange, m_nRequestID++ );
	}

	// RspQryExchange
	virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryExchange:");
		if (NULL != pExchange)
		{
			printf("%s|%s", 
				pExchange->ExchangeID,					// ½»Ò×Ëù´úÂë
				pExchange->ExchangeName);				// ½»Ò×ËùÃû³Æ
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// RspQryInstrument
			CThostFtdcQryInstrumentField QryInstrument;
			memset(&QryInstrument, 0, sizeof(QryInstrument));
			// exchange id
			strcpy(QryInstrument.ExchangeID, "SHFE");
			strcpy(QryInstrument.InstrumentID, m_chContract);

			m_pUserApi->ReqQryInstrument(&QryInstrument, m_nRequestID++ );
		}
	}

	// RspQryInstrument
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryInstrument:");
		if (NULL != pInstrument)
		{
			printf("%s|%s|%s|%d|%s|%s|%.04f|%d|", 
				pInstrument->ExchangeID,							// ½»Ò×Ëù´úÂë
				pInstrument->InstrumentID,							// ºÏÔ¼´úÂë
				pInstrument->InstrumentName,						// ºÏÔ¼Ãû³Æ
				pInstrument->VolumeMultiple,						// ºÏÔ¼ÊýÁ¿³ËÊý
				pInstrument->ExpireDate,							// µ½ÆÚÈÕ
				pInstrument->ProductID,								// ²úÆ·´úÂë
				pInstrument->PriceTick,								// ×îÐ¡±ä¶¯¼ÛÎ»
				nRequestID);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// QryInvestorPositionDetail
			CThostFtdcQryInvestorPositionDetailField InvestorPositionDetail;
			memset(&InvestorPositionDetail, 0, sizeof(InvestorPositionDetail));
			// broker id 
			strcpy(InvestorPositionDetail.BrokerID, m_chBrokerID);
			// investor id
			strcpy(InvestorPositionDetail.InvestorID, m_chUserID);

			m_pUserApi->ReqQryInvestorPositionDetail(&InvestorPositionDetail, m_nRequestID++ );
		}
	}

	// QryInvestorPositionDetail response
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryInvestorPositionDetail:");
		if (NULL != pInvestorPositionDetail)
		{
			printf("%s|%s|%s|%s|%s|%c|%c|%d|%.04f|%.04f|%.04f|",
				pInvestorPositionDetail->TradingDay,			// ½»Ò×ÈÕ
				pInvestorPositionDetail->OpenDate,				// ¿ª²ÖÈÕÆÚ
				pInvestorPositionDetail->TradeID,				// ³É½»±àºÅ
				pInvestorPositionDetail->InvestorID,			// Í¶×ÊÕß´úÂë
				pInvestorPositionDetail->ExchangeID,			// ½»Ò×Ëù´úÂë
				pInvestorPositionDetail->Direction,				// ÂòÂô±êÖ¾
				pInvestorPositionDetail->HedgeFlag,				// Í¶±£±êÖ¾
				pInvestorPositionDetail->Volume,				// ÊýÁ¿
				pInvestorPositionDetail->OpenPrice,				// ¿ª²Ö¼Û¸ñ
				pInvestorPositionDetail->Margin,				// Í¶×ÊÕß±£Ö¤½ð
				pInvestorPositionDetail->ExchMargin				// ½»Ò×Ëù±£Ö¤½ð
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// QryInstrumentMarginRate
			CThostFtdcQryInstrumentMarginRateField InstrumentMarginRate;
			memset(&InstrumentMarginRate, 0, sizeof(InstrumentMarginRate));
			// broker id 
			strcpy(InstrumentMarginRate.BrokerID, m_chBrokerID);
			// investor id
			strcpy(InstrumentMarginRate.InvestorID, m_chUserID);
			// instrument id
			strcpy(InstrumentMarginRate.InstrumentID, m_chContract);
			//strcpy(InstrumentMarginRate.InstrumentID, "");
			m_pUserApi->ReqQryInstrumentMarginRate(&InstrumentMarginRate, m_nRequestID++ );
		}
	}

	// QryInstrumentMarginRate response
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryInstrumentMarginRate:");
		if (NULL != pInstrumentMarginRate)
		{
			printf("%s|%s|%.04f|%.04f|%.04f|%.04f|",
				pInstrumentMarginRate->InvestorID,						// Í¶×ÊÕß´úÂë
				pInstrumentMarginRate->InstrumentID,					// ºÏÔ¼´úÂë
				pInstrumentMarginRate->LongMarginRatioByMoney,			// ¶àÍ·±£Ö¤½ðÂÊ
				pInstrumentMarginRate->LongMarginRatioByVolume,			// ¶àÍ·±£Ö¤½ð·Ñ
				pInstrumentMarginRate->ShortMarginRatioByMoney,			// ¿ÕÍ·±£Ö¤½ðÂÊ
				pInstrumentMarginRate->ShortMarginRatioByVolume);		// ¿ÕÍ·±£Ö¤½ð·Ñ
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);


		if (bIsLast == true)
		{
			// QryInstrumentCommissionRate
			CThostFtdcQryInstrumentCommissionRateField InstrumentCommissionRate;
			memset(&InstrumentCommissionRate, 0, sizeof(InstrumentCommissionRate));
			// broker id 
			strcpy(InstrumentCommissionRate.BrokerID, m_chBrokerID);
			// investor id
			strcpy(InstrumentCommissionRate.InvestorID, m_chUserID);
			// instrument id
			strcpy(InstrumentCommissionRate.InstrumentID, m_chContract);
			m_pUserApi->ReqQryInstrumentCommissionRate(&InstrumentCommissionRate, m_nRequestID++ );
		}
	}

	// QryInstrumentCommissionRate response
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryInstrumentCommissionRate:");
#if 0
		if (NULL != pInstrumentCommissionRate)
		{
			printf("%s|%s|%.04f|%.04f|%.04f|%.04f|%.04f|%.04f|",
				pInstrumentCommissionRate->InvestorID,						// Í¶×ÊÕß´úÂë
				pInstrumentCommissionRate->InstrumentID,					// ºÏÔ¼´úÂë
				pInstrumentCommissionRate->OpenRatioByMoney,				// ¿ª²ÖÊÖÐø·ÑÂÊ
				pInstrumentCommissionRate->OpenRatioByVolume,				// ¿ª²ÖÊÖÐø·Ñ
				pInstrumentCommissionRate->CloseRatioByMoney,				// Æ½²ÖÊÖÐø·ÑÂÊ
				pInstrumentCommissionRate->CloseRatioByVolume,				// Æ½²ÖÊÖÐø·Ñ
				pInstrumentCommissionRate->CloseTodayRatioByMoney,			// Æ½½ñÊÖÐø·ÑÂÊ
				pInstrumentCommissionRate->CloseTodayRatioByVolume);		// Æ½½ñÊÖÐø·Ñ
		}
#endif
		if (NULL != pInstrumentCommissionRate)
		{
			printf("%s|%s|%.08f|%.08f|%.08f|%.08f|%.08f|%.08f|",
				pInstrumentCommissionRate->InvestorID,						// Í¶×ÊÕß´úÂë
				pInstrumentCommissionRate->InstrumentID,					// ºÏÔ¼´úÂë
				pInstrumentCommissionRate->OpenRatioByMoney,				// ¿ª²ÖÊÖÐø·ÑÂÊ
				pInstrumentCommissionRate->OpenRatioByVolume,				// ¿ª²ÖÊÖÐø·Ñ
				pInstrumentCommissionRate->CloseRatioByMoney,				// Æ½²ÖÊÖÐø·ÑÂÊ
				pInstrumentCommissionRate->CloseRatioByVolume,				// Æ½²ÖÊÖÐø·Ñ
				pInstrumentCommissionRate->CloseTodayRatioByMoney,			// Æ½½ñÊÖÐø·ÑÂÊ
				pInstrumentCommissionRate->CloseTodayRatioByVolume);		// Æ½½ñÊÖÐø·Ñ
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// QryDepthMarketData 
			CThostFtdcQryDepthMarketDataField DepthMarketData;
			memset(&DepthMarketData, 0, sizeof(DepthMarketData));
			// instrument ID 
			strcpy(DepthMarketData.InstrumentID, m_chContract);

			m_pUserApi->ReqQryDepthMarketData(&DepthMarketData, m_nRequestID++ );
		}
	}

	// output the DepthMarketData result 
	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryDepthMarketData:");
		if(pDepthMarketData != NULL)
		{
			printf("%s|%s|%.04f|%.04f|%.04f|%.04f|%.04f|%d|%.04f|%.04f|%.04f|%d|%d|%.04f|%.04f|%.04f|%.04f|%.04f|%s|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|",
				pDepthMarketData->ExchangeID,					// ½»Ò×Ëù´úÂë
				pDepthMarketData->InstrumentID,					// ºÏÔ¼´úÂë
				pDepthMarketData->PreClosePrice,				// ×òÊÕÅÌ
				pDepthMarketData->OpenPrice,					// ½ñ¿ªÅÌ
				pDepthMarketData->HighestPrice,					// ×î¸ß¼Û
				pDepthMarketData->LowestPrice,					// ×îµÍ¼Û
				pDepthMarketData->LastPrice,					// ×îÐÂ¼Û
				pDepthMarketData->Volume,						// ÊýÁ¿
				pDepthMarketData->Turnover,						// ³É½»½ð¶î
				pDepthMarketData->BidPrice1,					// ÉêÂò¼ÛÒ»
				pDepthMarketData->AskPrice1,					// ÉêÂô¼ÛÒ»
				pDepthMarketData->BidVolume1,					// ÉêÂòÁ¿Ò»
				pDepthMarketData->AskVolume1,					// ÉêÂôÁ¿Ò»
				pDepthMarketData->UpperLimitPrice,				// ÕÇÍ£°å¼Û
				pDepthMarketData->LowerLimitPrice,				// µøÍ£°å¼Û
				pDepthMarketData->PreSettlementPrice,			// ÉÏ´Î½áËã¼Û
				pDepthMarketData->SettlementPrice,				// ±¾´Î½áËã¼Û
				pDepthMarketData->OpenInterest,					// ³Ö²ÖÁ¿
				pDepthMarketData->TradingDay,					// ½»Ò×ÈÕ
				pDepthMarketData->BidVolume2,					// ÉêÂòÁ¿¶þ
				pDepthMarketData->BidPrice2,					// ÉêÂò¼Û¶þ
				pDepthMarketData->BidVolume3,					// ÉêÂòÁ¿Èý
				pDepthMarketData->BidPrice3,					// ÉêÂò¼ÛÈý
				pDepthMarketData->BidVolume4,					// ÉêÂòÁ¿ËÄ
				pDepthMarketData->BidPrice4,					// ÉêÂò¼ÛËÄ
				pDepthMarketData->BidVolume5,					// ÉêÂòÁ¿Îå
				pDepthMarketData->BidPrice5,					// ÉêÂò¼ÛÎå
				pDepthMarketData->AskVolume2,					// ÉêÂôÁ¿¶þ
				pDepthMarketData->AskPrice2,					// ÉêÂô¼Û¶þ
				pDepthMarketData->AskVolume3,					// ÉêÂôÁ¿Èý
				pDepthMarketData->AskPrice3,					// ÉêÂô¼ÛÈý
				pDepthMarketData->AskVolume4,					// ÉêÂôÁ¿ËÄ
				pDepthMarketData->AskPrice4,					// ÉêÂô¼ÛËÄ
				pDepthMarketData->AskVolume5,					// ÉêÂôÁ¿Îå
				pDepthMarketData->AskPrice5						// ÉêÂô¼ÛÎå
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		//return;

		if (bIsLast == true)
		{
			// send order insertion request.
			CThostFtdcInputOrderField ord;
			memset(&ord, 0, sizeof(ord));
			// broker id 
			strcpy(ord.BrokerID, m_chBrokerID); 
			// investor ID 
			strcpy(ord.InvestorID, m_chUserID); 
			// instrument ID 
			strcpy(ord.InstrumentID, m_chContract);
			//ÏÂµ¥ÐèÌî½»Ò×Ëù´úÂë
			strcpy(ord.ExchangeID, "SHFE");
			///order reference 
			strcpy(ord.OrderRef, "9");
			// user id 
			strcpy(ord.UserID, m_chUserID); 
			// order price type 
			ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice; 
			// direction 
			ord.Direction = THOST_FTDC_D_Buy; 
			// combination order¡¯s offset flag 
			ord.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
			//strcpy(ord.CombOffsetFlag, "0"); 
			// combination or hedge flag 
			ord.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
			//strcpy(ord.CombHedgeFlag, "0");
			// price 
			ord.LimitPrice = 17000; 
			// volume 
			ord.VolumeTotalOriginal = 1; 
			// valid date 
			ord.TimeCondition = THOST_FTDC_TC_GFD; 
			// GTD DATE 
			strcpy(ord.GTDDate, ""); 
			// volume condition 
			ord.VolumeCondition = THOST_FTDC_VC_AV; 
			// min volume 
			ord.MinVolume = 0; 
			// trigger condition 
			ord.ContingentCondition = THOST_FTDC_CC_Immediately; 
			// stop price 
			ord.StopPrice = 0; 
			// force close reason 
			ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; 
			// auto suspend flag 
			ord.IsAutoSuspend = 0;
			// request id
			ord.RequestID = m_nRequestID;

			m_pUserApi->ReqOrderInsert(&ord, m_nRequestID++ );
		}

		if (bIsLast == true)
		{
			// qryorder
			CThostFtdcQryOrderField QryOrder;
			memset(&QryOrder, 0, sizeof(QryOrder));
			// broker id 
			strcpy(QryOrder.BrokerID, m_chBrokerID);
			// investor ID 
			strcpy(QryOrder.InvestorID, m_chUserID);

			m_pUserApi->ReqQryOrder(&QryOrder, m_nRequestID++ );
		}

	}
	// order insertion response 
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int  nRequestID, bool bIsLast) 
	{
		printf("OnRspOrderInsert:");
		if (NULL != pInputOrder)
		{
			printf("%s", pInputOrder->OrderRef);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

	}; 

	// order insertion return 
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) 
	{
		printf("OnRtnOrder:");
		if (NULL != pOrder)
		{
			printf("%d|%s|OrderSysID:%s|OrderLocalID:%s|%s|%s|%c|%s|%c|%s|%s|%d|%.04f|%d|%d|%s|%s|%s|%c|%c|%c|%c|%.04f|%s|%s|",
				pOrder->SequenceNo,							// ÐòºÅ	
				pOrder->InvestorID,							// ¿Í»§ºÅ
				pOrder->OrderSysID,							// Î¯ÍÐºÅ
				pOrder->OrderLocalID,						// ±¾µØ±¨µ¥±àºÅ
				pOrder->ExchangeID,							// ½»Ò×Ëù´úÂë
				pOrder->InstrumentID,						// ºÏÔ¼ºÅ
				pOrder->OrderStatus,						// ±¨µ¥×´Ì¬
				pOrder->StatusMsg,							// ×´Ì¬ÐÅÏ¢
				pOrder->Direction,							// ÂòÂô±ê¼Ç
				pOrder->CombOffsetFlag,						// ¿ªÆ½²Ö±êÖ¾
				pOrder->CombHedgeFlag,						// Í¶±£±ê¼Ç
				pOrder->VolumeTotalOriginal,				// Î¯ÍÐÊýÁ¿
				pOrder->LimitPrice,							// Î¯ÍÐ¼Û¸ñ
				pOrder->VolumeTraded,						// ³É½»ÊýÁ¿
				pOrder->VolumeTotal,						// Î´³É½»ÊýÁ¿
				pOrder->TradingDay,							// ½»¸îÆÚ
				pOrder->InsertTime,							// Î¯ÍÐÊ±¼ä
				pOrder->CancelTime,							// ³·µ¥Ê±¼ä
				pOrder->OrderType,							// ±¨µ¥ÀàÐÍ
				pOrder->OrderSource,						// ±¨µ¥À´Ô´
				pOrder->OrderPriceType,						// ±¨µ¥¼Û¸ñÌõ¼þ
				pOrder->TimeCondition,						// ÓÐÐ§ÆÚÀàÐÍ
				pOrder->StopPrice,							// Ö¹Ëð¼Û
				pOrder->ActiveTime,							// ¼¤»îÊ±¼ä
				pOrder->OrderRef							// ±¨µ¥ÒýÓÃ
				); 
		}
		printf("\n");
		printf("RequestID=[%d]\n", pOrder->RequestID);

		// order insertion success, then send order action request.
		if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing && atoi(pOrder->OrderSysID) != 0)
		{
			// delete ord
			CThostFtdcInputOrderActionField ord;
			memset(&ord, 0, sizeof(ord));
			// broker id 
			strcpy(ord.BrokerID, m_chBrokerID); 
			// investor ID 
			strcpy(ord.InvestorID, m_chUserID);
			// order action refernce
			ord.OrderActionRef=000000000001;

			/// order reference 
			strcpy(ord.OrderRef, pOrder->OrderRef);		// *±Ø´«1
			// front id
			ord.FrontID = pOrder->FrontID;				// *±Ø´«2
			// session id
			ord.SessionID = pOrder->SessionID;			// *±Ø´«3

			// exchange ID
			strcpy(ord.ExchangeID, pOrder->ExchangeID);	// *±Ø´«4
			// ±¨µ¥µ½½»Ò×Ëùºó¿ÉÓÃExchangeID+OrderSysID³·µ¥£¬·ñÔòÓÃOrderRef+FrontID+SessionID+ExchangeID³·µ¥
			// action order ID
			strcpy(ord.OrderSysID,pOrder->OrderSysID);	

			//  action order num(unavailable yet)
			ord.VolumeChange=0;
			// instrument ID 
			strcpy(ord.InstrumentID, pOrder->InstrumentID); 
			// user id 
			strcpy(ord.UserID, m_chUserID); 

			m_pUserApi->ReqOrderAction(&ord, m_nRequestID++ ); 
		}
	}

	///trade return
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade)
	{
		static int s_nTotalBuy = 0;
		static int s_nTotalSell = 0;

		printf("OnRtnTrade:");
		if (NULL != pTrade)
		{
			if (pTrade->Direction == THOST_FTDC_D_Buy)
				s_nTotalBuy += pTrade->Volume;
			else if (pTrade->Direction == THOST_FTDC_D_Sell)
				s_nTotalSell += pTrade->Volume;
			else
				printf ("invalid direction:%c\n", pTrade->Direction);

			printf("%d|%s|%s|%s|%s|³É½»|%c|%c|%c|%d|%.04f|%s|%s|%s|%s|s_nTotalBuy=%d|s_nTotalSell=%d|",
				pTrade->SequenceNo,					// ÐòºÅ
				pTrade->InvestorID,					// ¿Í»§ºÅ
				pTrade->ExchangeID,					// ½»Ò×Ëù´úÂë
				pTrade->OrderSysID,					// Î¯ÍÐµ¥ºÅ
				pTrade->InstrumentID,				// ºÏÔ¼±àÂë
				pTrade->Direction,					// ÂòÂô±ê¼Ç
				pTrade->OffsetFlag,					// ¿ªÆ½²Ö±êÖ¾
				pTrade->HedgeFlag,					// Í¶±£±ê¼Ç
				pTrade->Volume,						// ³É½»ÊýÁ¿
				pTrade->Price,						// ³É½»¼Û¸ñ
				pTrade->TradeID,					// ³É½»ºÅ
				pTrade->TradingDay,					// ½»¸îÆÚ
				pTrade->TradeTime,					// ³É½»Ê±¼ä
				pTrade->OrderRef,					// ±¨µ¥ÒýÓÃ
				s_nTotalBuy,
				s_nTotalSell
				);
		}
		printf("\n");
	}

	// output the order action result 
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspOrderAction:");
		if (NULL != pInputOrderAction)
		{
			printf("%s|OrderSysID:%s|%s|%s|%.04f|",
				pInputOrderAction->InvestorID,							// ¿Í»§ºÅ
				pInputOrderAction->OrderSysID,							// Î¯ÍÐºÅ
				pInputOrderAction->ExchangeID,							// ½»Ò×Ëù´úÂë
				pInputOrderAction->InstrumentID,						// ºÏÔ¼ºÅ
				pInputOrderAction->LimitPrice							// Î¯ÍÐ¼Û¸ñ
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	// qryorder return
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryOrder:");
		if(pOrder != NULL)
		{
			printf("%s", pOrder->OrderSysID);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{	
			// qrytrade
			CThostFtdcQryTradeField QryTrade;
			memset(&QryTrade, 0, sizeof(QryTrade));
			// broker id 
			strcpy(QryTrade.BrokerID, m_chBrokerID); 
			// investor ID 
			strcpy(QryTrade.InvestorID, m_chUserID);
			// start time
			strcpy(QryTrade.TradeTimeStart, "20110530");
			// end time
			strcpy(QryTrade.TradeTimeEnd, "20110601");
			// exchange id
			strcpy(QryTrade.ExchangeID, "SHFE");
			// instructment id
			strcpy(QryTrade.InstrumentID, m_chContract);

			m_pUserApi->ReqQryTrade(&QryTrade, m_nRequestID++ );
		}
	}

	// qrytrade return
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryTrade:");
		if(pTrade != NULL)
		{
			printf("%s|%s|%s|%s|%c|%c|%c|%d|%.04f|%s|%s|%s|%s|%d",
				pTrade->InvestorID,							// ¿Í»§ºÅ
				pTrade->ExchangeID,							// ½»Ò×Ëù´úÂë
				pTrade->OrderSysID,							// ±¨µ¥±àºÅ
				pTrade->InstrumentID,						// ºÏÔ¼´úÂë
				pTrade->Direction,							// ÂòÂô±ê¼Ç
				pTrade->OffsetFlag,							// ¿ªÆ½±êÖ¾
				pTrade->HedgeFlag,							// Í¶±£±ê¼Ç
				pTrade->Volume,								// ³É½»ÊýÁ¿
				pTrade->Price,								// ³É½»¼Û¸ñ
				pTrade->TradeID,							// ³É½»ºÅ
				pTrade->TradeDate,							// ÈÕÆÚ
				pTrade->TradingDay,							// ½»Ò×ÈÕ
				pTrade->TradeTime,							// ³É½»Ê±¼ä
				pTrade->SequenceNo							// ÐòºÅ
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// qry position
			CThostFtdcQryInvestorPositionField QryInvestorPosition;
			memset(&QryInvestorPosition, 0, sizeof(QryInvestorPosition));
			// broker id 
			strcpy(QryInvestorPosition.BrokerID, m_chBrokerID); 
			// investor ID 
			strcpy(QryInvestorPosition.InvestorID, m_chUserID);
			// instrument id
			strcpy(QryInvestorPosition.InstrumentID, m_chContract);

			m_pUserApi->ReqQryInvestorPosition(&QryInvestorPosition, m_nRequestID++ );;
		}
	}

	// QryInvestorPosition return
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryInvestorPosition:");
		if(pInvestorPosition != NULL)
		{
			printf("%s|%d|%d|%.04f|%.04f|%s|%c|%.04f|%c|%s|%.04f|", 
				pInvestorPosition->InvestorID,					// ¿Í»§ºÅ
				pInvestorPosition->Position,					// ½ñÈÕ×Ü³Ö²Ö
				pInvestorPosition->TodayPosition,				// ½ñÈÕÏÖ³Ö²Ö
				pInvestorPosition->PositionCost,				// ³Ö²Ö³É±¾	
				pInvestorPosition->OpenCost,					// ¿ª²Ö³É±¾
				pInvestorPosition->InstrumentID,				// ºÏÔ¼´úÂë
				pInvestorPosition->HedgeFlag,					// Í¶»úÌ×±£±êÖ¾
				pInvestorPosition->PositionProfit,				// ³Ö²ÖÓ¯¿÷
				pInvestorPosition->PosiDirection,				// ³Ö²Ö¶à¿Õ·½Ïò
				pInvestorPosition->TradingDay,					// ½»Ò×ÈÕ
				pInvestorPosition->UseMargin					// Õ¼ÓÃµÄ±£Ö¤½ð
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{	
			// qry CFMMC key
			CThostFtdcQryCFMMCTradingAccountKeyField CFMMCTradingAccountKey;
			memset(&CFMMCTradingAccountKey, 0, sizeof(CFMMCTradingAccountKey));
			// broker id
			strcpy(CFMMCTradingAccountKey.BrokerID,m_chBrokerID);
			// user id
			strcpy(CFMMCTradingAccountKey.InvestorID, m_chUserID);

			m_pUserApi->ReqQryCFMMCTradingAccountKey(&CFMMCTradingAccountKey, m_nRequestID++ );
		}
	}

	// QryCFMMCTradingAccountKey return
	virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryCFMMCTradingAccountKey:");
		if (NULL != pCFMMCTradingAccountKey)
		{
			printf("%s|%s|%d|%s|",
				pCFMMCTradingAccountKey->AccountID,				   // Í¶×ÊÕßÕÊºÅ
				pCFMMCTradingAccountKey->ParticipantID,			   // ¾­¼Í¹«Ë¾Í³Ò»±àÂë
				pCFMMCTradingAccountKey->KeyID,					   // ÃÜÔ¿±àºÅ
				pCFMMCTradingAccountKey->CurrentKey				   // ¶¯Ì¬ÃÜÔ¿
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// password update
			CThostFtdcUserPasswordUpdateField UserPasswordUpdate;
			memset(&UserPasswordUpdate, 0, sizeof(UserPasswordUpdate));
			// broker id
			strcpy(UserPasswordUpdate.BrokerID,m_chBrokerID);
			// user id
			strcpy(UserPasswordUpdate.UserID, m_chUserID);
			// old password
			strcpy(UserPasswordUpdate.OldPassword, "123456");
			// new password
			strcpy(UserPasswordUpdate.NewPassword, "123456");

			m_pUserApi->ReqUserPasswordUpdate(&UserPasswordUpdate, m_nRequestID++ );
		}
	}

	// PasswordUpdate return
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspUserPasswordUpdate:");
		if (NULL != pUserPasswordUpdate)
		{
			printf("%s|%s|%s|",
				pUserPasswordUpdate->UserID,			// ¿Í»§ºÅ
				pUserPasswordUpdate->OldPassword,		// ¾É¿ÚÁî
				pUserPasswordUpdate->NewPassword		// ÐÂ¿ÚÁî
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// account password update
			CThostFtdcTradingAccountPasswordUpdateField TradingAccountPasswordUpdate ;
			memset(&TradingAccountPasswordUpdate, 0, sizeof(TradingAccountPasswordUpdate));
			// broker id
			strcpy(TradingAccountPasswordUpdate.BrokerID,m_chBrokerID);
			// account id
			strcpy(TradingAccountPasswordUpdate.AccountID, m_chUserID);
			// old password
			strcpy(TradingAccountPasswordUpdate.OldPassword, "123456");
			// new password
			strcpy(TradingAccountPasswordUpdate.NewPassword, "123456");

			m_pUserApi->ReqTradingAccountPasswordUpdate(&TradingAccountPasswordUpdate, m_nRequestID++ );
		}
	}

	// TradingAccountPasswordUpdate
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspTradingAccountPasswordUpdate:");
		if (NULL != pTradingAccountPasswordUpdate)
		{
			printf("%s|%s|%s|",
				pTradingAccountPasswordUpdate->AccountID,				// ×Ê½ðÕË»§
				pTradingAccountPasswordUpdate->OldPassword,				// ¾É¿ÚÁî
				pTradingAccountPasswordUpdate->NewPassword				// ÐÂ¿ÚÁî
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// SettlementInfo Confirm
			CThostFtdcQrySettlementInfoConfirmField SettlementInfoConfirm;
			memset(&SettlementInfoConfirm, 0, sizeof(SettlementInfoConfirm));
			// broker id
			strcpy(SettlementInfoConfirm.BrokerID, m_chBrokerID);
			// investor ID
			strcpy(SettlementInfoConfirm.InvestorID, m_chUserID);

			m_pUserApi->ReqQrySettlementInfoConfirm(&SettlementInfoConfirm, m_nRequestID++ );
		}
	}

	// QrySettlementInfoConfirm return
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQrySettlementInfoConfirm:");
		if (NULL != pSettlementInfoConfirm)
		{
			printf("%s|%s|",
				pSettlementInfoConfirm->InvestorID,					// ¿Í»§ºÅ
				pSettlementInfoConfirm->ConfirmDate					// È·ÈÏÈÕÆÚ
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// QrySettlementInfo
			CThostFtdcQrySettlementInfoField QrySettlementInfo;
			memset(&QrySettlementInfo, 0, sizeof(QrySettlementInfo));
			// broker id 
			strcpy(QrySettlementInfo.BrokerID, m_chBrokerID);
			// investor ID 
			strcpy(QrySettlementInfo.InvestorID, m_chUserID);
			// trading day
			strcpy(QrySettlementInfo.TradingDay, "");

			m_pUserApi->ReqQrySettlementInfo(&QrySettlementInfo, m_nRequestID++ );
		}
	}

	///QrySettlementInfo return
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQrySettlementInfoConfirm:");
		if(pSettlementInfo != NULL)
		{
			printf("%s|%s|",
				pSettlementInfo->InvestorID,			// ¿Í»§ºÅ
				pSettlementInfo->Content				// ÏûÏ¢ÕýÎÄ
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// QrySettlementInfo
			CThostFtdcQryInvestorPositionCombineDetailField InvestorPositionDetail;
			memset(&InvestorPositionDetail, 0, sizeof(InvestorPositionDetail));
			// broker id 
			strcpy(InvestorPositionDetail.BrokerID, m_chBrokerID); 
			// investor ID 
			strcpy(InvestorPositionDetail.InvestorID, m_chUserID);
			// 
			strcpy(InvestorPositionDetail.CombInstrumentID, "fu1109");

			m_pUserApi->ReqQryInvestorPositionCombineDetail(&InvestorPositionDetail, m_nRequestID++ );
		}
	}

	// QryInvestorPositionCombineDetail return
	virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryInvestorPositionCombineDetail:");
		if(pInvestorPositionCombineDetail != NULL)
		{
			printf("%s|%s|%s|%d|%.04f|",
				pInvestorPositionCombineDetail->InvestorID,					// ¿Í»§ºÅ
				pInvestorPositionCombineDetail->ExchangeID,					// ½»Ò×Ëù´úÂë
				pInvestorPositionCombineDetail->CombInstrumentID,			// ×éºÏºÏÔ¼
				pInvestorPositionCombineDetail->TotalAmt,					// ³Ö²ÖÁ¿
				pInvestorPositionCombineDetail->Margin						// Í¶×ÊÕß±£Ö¤½ð
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// BankToFuture
			CThostFtdcReqTransferField ReqTransfer;
			memset(&ReqTransfer, 0, sizeof(ReqTransfer));
			// broker id 
			strcpy(ReqTransfer.BrokerID, m_chBrokerID); 
			// AccountID ID 
			strcpy(ReqTransfer.AccountID, m_chUserID);
			// BankPassWord
			strcpy(ReqTransfer.BankPassWord, "123");
			//Password
			strcpy(ReqTransfer.Password, "123");
			// TradeAmount
			ReqTransfer.TradeAmount = 100;
			// BankID
			strcpy(ReqTransfer.BankID, "2");
			// BankAccount
			strcpy(ReqTransfer.BankAccount, "360000016");

			m_pUserApi->ReqFromBankToFutureByFuture(&ReqTransfer, m_nRequestID++ );
		}
	}

	///ÆÚ»õ·¢ÆðÒøÐÐ×Ê½ð×ªÆÚ»õÓ¦´ð
	virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
	{
		printf("OnRspFromBankToFutureByFuture:");
		if (NULL != pReqTransfer)
		{
			printf("%s|%s|%s|%s|%d|%.04f|%c|%c|",
				pReqTransfer->AccountID,						// Í¶×ÊÕßÕËºÅ
				pReqTransfer->BankID,							// ÒøÐÐ´úÂë
				pReqTransfer->BankAccount,						// ÒøÐÐÕËºÅ
				pReqTransfer->TradeTime,						// ²Ù×÷Ê±¼ä
				pReqTransfer->PlateSerial,						// ÒøÆÚÆ½Ì¨Á÷Ë®ºÅ
				pReqTransfer->TradeAmount,						// ×ªÕË½ð¶î
				pReqTransfer->TransferStatus,					// ´¦Àí×´Ì¬
				pReqTransfer->BankAccType						// ÒøÐÐÀàÐÍ
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// FutureToBan
			CThostFtdcReqTransferField ReqTransfer;
			memset(&ReqTransfer, 0, sizeof(ReqTransfer));
			// broker id 
			strcpy(ReqTransfer.BrokerID, m_chBrokerID); 
			// AccountID ID 
			strcpy(ReqTransfer.AccountID, m_chUserID);
			// BankPassWord
			strcpy(ReqTransfer.BankPassWord, "123");
			// TradeAmount
			ReqTransfer.TradeAmount = 100;
			// BankID
			strcpy(ReqTransfer.BankID, "2");
			// BankAccount
			strcpy(ReqTransfer.BankAccount, "360000016");

			m_pUserApi->ReqFromFutureToBankByFuture(&ReqTransfer, m_nRequestID++ );
		}
	}

	///ÆÚ»õ·¢ÆðÆÚ»õ×Ê½ð×ªÒøÐÐÓ¦´ð
	virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspFromFutureToBankByFuture:");
		if (NULL != pReqTransfer)
		{
			printf("%s|%s|%s|%s|%d|%.04f|%c|%c|",
				pReqTransfer->AccountID,						// Í¶×ÊÕßÕËºÅ
				pReqTransfer->BankID,							// ÒøÐÐ´úÂë
				pReqTransfer->BankAccount,						// ÒøÐÐÕËºÅ
				pReqTransfer->TradeTime,						// ²Ù×÷Ê±¼ä
				pReqTransfer->PlateSerial,						// ÒøÆÚÆ½Ì¨Á÷Ë®ºÅ
				pReqTransfer->TradeAmount,						// ×ªÕË½ð¶î
				pReqTransfer->TransferStatus,					// ´¦Àí×´Ì¬
				pReqTransfer->BankAccType						// ÒøÐÐÀàÐÍ
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// FutureToBank
			CThostFtdcReqQueryAccountField ReqQueryAccount;
			memset(&ReqQueryAccount, 0, sizeof(ReqQueryAccount));
			// AccountID ID 
			strcpy(ReqQueryAccount.AccountID, m_chUserID);
			// BankPassWord
			strcpy(ReqQueryAccount.BankPassWord, "1234");
			// BankID
			strcpy(ReqQueryAccount.BankID, "2");
			// BankAccount
			strcpy(ReqQueryAccount.BankAccount, "360000016");

			m_pUserApi->ReqQueryBankAccountMoneyByFuture(&ReqQueryAccount, m_nRequestID++ );
		}
	}	

	///ÆÚ»õ·¢Æð²éÑ¯ÒøÐÐÓà¶îÓ¦´ð
	virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQueryBankAccountMoneyByFuture:");
		if (NULL != pReqQueryAccount)
		{
			printf("%s|%s|%s|%s|%d|%c|",
				pReqQueryAccount->AccountID,						// Í¶×ÊÕßÕËºÅ
				pReqQueryAccount->BankID,							// ÒøÐÐ´úÂë
				pReqQueryAccount->BankAccount,						// ÒøÐÐÕËºÅ
				pReqQueryAccount->TradeTime,						// ²Ù×÷Ê±¼ä
				pReqQueryAccount->PlateSerial,						// ÒøÆÚÆ½Ì¨Á÷Ë®ºÅ
				pReqQueryAccount->BankAccType						// ÒøÐÐÀàÐÍ
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// TransferSerial
			CThostFtdcQryTransferSerialField QryTransferSerial;
			memset(&QryTransferSerial, 0, sizeof(QryTransferSerial));
			// broker id 
			strcpy(QryTransferSerial.BrokerID, m_chBrokerID); 
			// investor ID 
			strcpy(QryTransferSerial.AccountID, m_chUserID);
			// BankID
			strcpy(QryTransferSerial.BankID, "5");

			m_pUserApi->ReqQryTransferSerial(&QryTransferSerial, m_nRequestID++ );
		}
	}

	// TransferSerial return
	virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryTransferSerial:");
		if (pTransferSerial != NULL)
		{
			printf("%d|%s|%s|%s|%s|%s|%.04f|%s|%c|%c|",
				pTransferSerial->PlateSerial,				// ÒøÆÚÆ½Ì¨Á÷Ë®ºÅ
				pTransferSerial->AccountID,					// Í¶×ÊÕßÕËºÅ
				pTransferSerial->BankAccount,				// ÒøÐÐÕËºÅ
				pTransferSerial->OperatorCode,				// ²Ù×÷Ô±
				pTransferSerial->TradingDay,				// ²Ù×÷ÈÕÆÚ
				pTransferSerial->TradeTime,					// ²Ù×÷Ê±¼ä
				pTransferSerial->TradeAmount,				// ½»Ò×½ð¶î
				pTransferSerial->BankID,					// ÒøÐÐ´úÂë
				pTransferSerial->BankAccType,				// ÒøÐÐÀàÐÍ
				pTransferSerial->AvailabilityFlag			// ÓÐÐ§±êÖ¾
				);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			CThostFtdcQryNoticeField QryNotice;
			memset(&QryNotice, 0, sizeof(QryNotice));
			// broker id 
			strcpy(QryNotice.BrokerID, m_chBrokerID);

			m_pUserApi->ReqQryNotice(&QryNotice, m_nRequestID++ );
		}
	}

	// notice return
	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryNotice:");
		if (NULL != pNotice)
		{
			printf(":%s", pNotice->Content);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			CThostFtdcQryTradingNoticeField QryTradingNotice;
			memset(&QryTradingNotice, 0, sizeof(QryTradingNotice)-1);
			// broker id 
			strcpy(QryTradingNotice.BrokerID, m_chBrokerID);
			// investor ID
			strcpy(QryTradingNotice.InvestorID, m_chUserID);

			m_pUserApi->ReqQryTradingNotice(&QryTradingNotice, m_nRequestID++ );
		}
	}

	// trade notice return
	virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryTradingNotice:");
		if (NULL != pTradingNotice)
		{
			printf(":%s", pTradingNotice->FieldContent);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (bIsLast == true)
		{
			// logout
			CThostFtdcUserLogoutField UserLogout;
			memset(&UserLogout, 0, sizeof(UserLogout));
			// broker id 
			strcpy(UserLogout.BrokerID, m_chBrokerID); 
			// investor ID 
			strcpy(UserLogout.UserID, m_chUserID);
			m_pUserApi->ReqUserLogout(&UserLogout, m_nRequestID++ );
		}
	}

	// logout return
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspUserLogout:");
		if (NULL != pUserLogout)
		{
			printf("%s",pUserLogout->UserID);
		}
		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		return;
	}

	//²éÑ¯¿ªÅÌÇ°µÄ³Ö²ÖÃ÷Ï¸Ó¦´ð
	virtual void OnRspQryInvestorOpenPosition(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspQryInvestorOpenPosition:");

		printf("\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}
private:     
	// ÊµÀýÖ¸Õë
	CThostFtdcTraderApi *m_pUserApi;
	// ¾­¼Ã¹«Ë¾´úÂë
	TThostFtdcBrokerIDType m_chBrokerID;
	// ÓÃ»§Ãû
	TThostFtdcUserIDType m_chUserID;
	// ÓÃ»§ÃÜÂë
	TThostFtdcPasswordType m_chPassword;
	// ºÏÔ¼
	TThostFtdcContractCodeType m_chContract;
public:
	// ÇëÇóºÅ
	int m_nRequestID;
};

class CCosHandler:public CKSCosSpi
{
public:
	CCosHandler ()
	{
	}

	~CCosHandler ()
	{
	}

	//Ìõ¼þµ¥Â¼ÈëÏìÓ¦
	virtual void OnRspInitInsertConditionalOrder (CKSConditionalOrderOperResultField *pInitInsertConditionalOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
	{
		printf ("hello insert\n");
		if (pInitInsertConditionalOrder != NULL)
		{
			cout << "OnRspInitInsertConditionalOrder:"
				<< "¾­¼Ã¹«Ë¾´úÂë:" << pInitInsertConditionalOrder->BrokerID
				<< "¿Í»§ºÅ:" << pInitInsertConditionalOrder->InvestorID
				<< "Ìõ¼þµ¥±àºÅ:" << pInitInsertConditionalOrder->ConditionalOrderID
				<< "±¾µØ±¨µ¥ºÅ:" << pInitInsertConditionalOrder->OrderLocalID
				<< "½»Ò×Ëù´úÂë:" << pInitInsertConditionalOrder->ExchangeID
				<< "ºÏÔ¼±àÂë:" << pInitInsertConditionalOrder->InstrumentID
				<< "±¨µ¥×´Ì¬:" << pInitInsertConditionalOrder->OrderStatus
				<< "ÂòÂô±ê¼Ç:" << pInitInsertConditionalOrder->Direction
				<< "¿ªÆ½²Ö±êÖ¾:" << pInitInsertConditionalOrder->CombOffsetFlag
				<< "Í¶±£±ê¼Ç:" << pInitInsertConditionalOrder->CombHedgeFlag
				<< "Î¯ÍÐÊýÁ¿:" << pInitInsertConditionalOrder->VolumeTotalOriginal
				<< "Î¯ÍÐ¼Û¸ñ:" << pInitInsertConditionalOrder->LimitPrice
				<< "½»Ò×ÈÕ:" << pInitInsertConditionalOrder->TradingDay
				<< "³·ÏúÓÃ»§:" << pInitInsertConditionalOrder->UserID
				<< "³·ÏúÊ±¼ä:" << pInitInsertConditionalOrder->CancelTime
				<< "¿Í»§´úÂë:" << pInitInsertConditionalOrder->ClientID
				<< "Ìõ¼þµ¥×´Ì¬:" << pInitInsertConditionalOrder->ConditionalOrderStatus 
				<< "´íÎóÐÅÏ¢:" << pInitInsertConditionalOrder->ErrorMsg 
				<< "¼Û¸ñÀà±ð:" << pInitInsertConditionalOrder->OrderPriceType 
				<< "´¥·¢´ÎÊý:" << pInitInsertConditionalOrder->TriggeredTimes 
				<< "Ìõ¼þµ¥Àà±ð:" << pInitInsertConditionalOrder->OrderType 
				<< "±¸×¢:" << pInitInsertConditionalOrder->Memo 
				<< "ÓÐÐ§Ê±¼ä:" << pInitInsertConditionalOrder->ActiveTime 
				<< "Ê§Ð§Ê±¼ä:" << pInitInsertConditionalOrder->InActiveTime << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	//Ìõ¼þµ¥²éÑ¯ÏìÓ¦
	virtual void OnRspQueryConditionalOrder (CKSConditionalOrderOperResultField * pQueryConditionalOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
	{
		printf ("hello query\n");
		if (pQueryConditionalOrder != NULL)
		{
			cout << "OnRspQueryConditionalOrder:"
				<< "¾­¼Ã¹«Ë¾´úÂë:" << pQueryConditionalOrder->BrokerID
				<< "¿Í»§ºÅ:" << pQueryConditionalOrder->InvestorID
				<< "Ìõ¼þµ¥±àºÅ:" << pQueryConditionalOrder->ConditionalOrderID
				<< "±¾µØ±¨µ¥ºÅ:" << pQueryConditionalOrder->OrderLocalID
				<< "½»Ò×Ëù´úÂë:" << pQueryConditionalOrder->ExchangeID
				<< "ºÏÔ¼±àÂë:" << pQueryConditionalOrder->InstrumentID
				<< "±¨µ¥×´Ì¬:" << pQueryConditionalOrder->OrderStatus
				<< "ÂòÂô±ê¼Ç:" << pQueryConditionalOrder->Direction
				<< "¿ªÆ½²Ö±êÖ¾:" << pQueryConditionalOrder->CombOffsetFlag
				<< "Í¶±£±ê¼Ç:" << pQueryConditionalOrder->CombHedgeFlag
				<< "Î¯ÍÐÊýÁ¿:" << pQueryConditionalOrder->VolumeTotalOriginal
				<< "Î¯ÍÐ¼Û¸ñ:" << pQueryConditionalOrder->LimitPrice
				<< "½»Ò×ÈÕ:" << pQueryConditionalOrder->TradingDay
				<< "³·ÏúÓÃ»§:" << pQueryConditionalOrder->UserID
				<< "³·ÏúÊ±¼ä:" << pQueryConditionalOrder->CancelTime
				<< "¿Í»§´úÂë:" << pQueryConditionalOrder->ClientID
				<< "Ìõ¼þµ¥×´Ì¬:" << pQueryConditionalOrder->ConditionalOrderStatus
				<< "´íÎóÐÅÏ¢:" << pQueryConditionalOrder->ErrorMsg
				<< "¼Û¸ñÀà±ð:" << pQueryConditionalOrder->OrderPriceType
				<< "´¥·¢´ÎÊý:" << pQueryConditionalOrder->TriggeredTimes
				<< "Ìõ¼þµ¥Àà±ð:" << pQueryConditionalOrder->OrderType
				<< "±¸×¢:" << pQueryConditionalOrder->Memo
				<< "ÓÐÐ§Ê±¼ä:" << pQueryConditionalOrder->ActiveTime
				<< "Ê§Ð§Ê±¼ä:" << pQueryConditionalOrder->InActiveTime << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	//Ìõ¼þµ¥ÐÞ¸ÄÏìÓ¦
	virtual void OnRspModifyConditionalOrder (CKSConditionalOrderOperResultField *pModifyConditionalOrder,	CThostFtdcRspInfoField * pRspInfo,	int nRequestID, bool bIsLast)
	{
		printf ("hello modify\n");
		if (pModifyConditionalOrder != NULL)
		{
			cout << "OnRspModifyConditionalOrder:"
				<< "¾­¼Ã¹«Ë¾´úÂë:" << pModifyConditionalOrder->BrokerID
				<< "¿Í»§ºÅ:" << pModifyConditionalOrder->InvestorID
				<< "Ìõ¼þµ¥±àºÅ:" << pModifyConditionalOrder->ConditionalOrderID
				<< "±¾µØ±¨µ¥ºÅ:" << pModifyConditionalOrder->OrderLocalID
				<< "½»Ò×Ëù´úÂë:" << pModifyConditionalOrder->ExchangeID
				<< "ºÏÔ¼±àÂë:" << pModifyConditionalOrder->InstrumentID
				<< "±¨µ¥×´Ì¬:" << pModifyConditionalOrder->OrderStatus
				<< "ÂòÂô±ê¼Ç:" << pModifyConditionalOrder->Direction
				<< "¿ªÆ½²Ö±êÖ¾:" << pModifyConditionalOrder->CombOffsetFlag
				<< "Í¶±£±ê¼Ç:" << pModifyConditionalOrder->CombHedgeFlag
				<< "Î¯ÍÐÊýÁ¿:" << pModifyConditionalOrder->VolumeTotalOriginal
				<< "Î¯ÍÐ¼Û¸ñ:" << pModifyConditionalOrder->LimitPrice
				<< "½»Ò×ÈÕ:" << pModifyConditionalOrder->TradingDay
				<< "³·ÏúÓÃ»§:" << pModifyConditionalOrder->UserID
				<< "³·ÏúÊ±¼ä:" << pModifyConditionalOrder->CancelTime
				<< "¿Í»§´úÂë:" << pModifyConditionalOrder->ClientID
				<< "Ìõ¼þµ¥×´Ì¬:" << pModifyConditionalOrder->ConditionalOrderStatus
				<< "´íÎóÐÅÏ¢:" << pModifyConditionalOrder->ErrorMsg
				<< "¼Û¸ñÀà±ð:" << pModifyConditionalOrder->OrderPriceType
				<< "´¥·¢´ÎÊý:" << pModifyConditionalOrder->TriggeredTimes
				<< "Ìõ¼þµ¥Àà±ð:" << pModifyConditionalOrder->OrderType
				<< "±¸×¢:" << pModifyConditionalOrder->Memo
				<< "ÓÐÐ§Ê±¼ä:" << pModifyConditionalOrder->ActiveTime
				<< "Ê§Ð§Ê±¼ä:" << pModifyConditionalOrder->InActiveTime << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	//Ìõ¼þµ¥ÔÝÍ£¼¤»îÏìÓ¦
	virtual void OnRspPauseConditionalOrder (CKSConditionalOrderOperResultField *pPauseConditionalOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
	{
		printf ("hello pause or active\n");
		if (pPauseConditionalOrder != NULL)
		{
			cout << "OnRspPauseConditionalOrder:"
				<< "¾­¼Ã¹«Ë¾´úÂë:" << pPauseConditionalOrder->BrokerID
				<< "¿Í»§ºÅ:" << pPauseConditionalOrder->InvestorID
				<< "Ìõ¼þµ¥±àºÅ:" << pPauseConditionalOrder->ConditionalOrderID
				<< "±¾µØ±¨µ¥ºÅ:" << pPauseConditionalOrder->OrderLocalID
				<< "½»Ò×Ëù´úÂë:" << pPauseConditionalOrder->ExchangeID
				<< "ºÏÔ¼±àÂë:" << pPauseConditionalOrder->InstrumentID
				<< "±¨µ¥×´Ì¬:" << pPauseConditionalOrder->OrderStatus
				<< "ÂòÂô±ê¼Ç:" << pPauseConditionalOrder->Direction
				<< "¿ªÆ½²Ö±êÖ¾:" << pPauseConditionalOrder->CombOffsetFlag
				<< "Í¶±£±ê¼Ç:" << pPauseConditionalOrder->CombHedgeFlag
				<< "Î¯ÍÐÊýÁ¿:" << pPauseConditionalOrder->VolumeTotalOriginal
				<< "Î¯ÍÐ¼Û¸ñ:" << pPauseConditionalOrder->LimitPrice
				<< "½»Ò×ÈÕ:" << pPauseConditionalOrder->TradingDay
				<< "³·ÏúÓÃ»§:" << pPauseConditionalOrder->UserID
				<< "³·ÏúÊ±¼ä:" << pPauseConditionalOrder->CancelTime
				<< "¿Í»§´úÂë:" << pPauseConditionalOrder->ClientID
				<< "Ìõ¼þµ¥×´Ì¬:" << pPauseConditionalOrder->ConditionalOrderStatus
				<< "´íÎóÐÅÏ¢:" << pPauseConditionalOrder->ErrorMsg
				<< "¼Û¸ñÀà±ð:" << pPauseConditionalOrder->OrderPriceType
				<< "´¥·¢´ÎÊý:" << pPauseConditionalOrder->TriggeredTimes
				<< "Ìõ¼þµ¥Àà±ð:" << pPauseConditionalOrder->OrderType
				<< "±¸×¢:" << pPauseConditionalOrder->Memo
				<< "ÓÐÐ§Ê±¼ä:" << pPauseConditionalOrder->ActiveTime
				<< "Ê§Ð§Ê±¼ä:" << pPauseConditionalOrder->InActiveTime << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

	}

	//Ìõ¼þµ¥É¾³ýÏìÓ¦
	virtual void OnRspRemoveConditionalOrder (CKSConditionalOrderRspResultField *pRemoveConditionalOrder,	CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
	{
		printf ("hello remove\n");
		if (pRemoveConditionalOrder != NULL)
		{
			cout << "OnRspRemoveConditionalOrder:"
				<< "ÓªÒµ²¿´úÂë:" << pRemoveConditionalOrder->BrokerID
				<< "Í¶×ÊÕß´úÂë:" << pRemoveConditionalOrder->InvestorID
				<< "Ìõ¼þµ¥±àºÅ:" << pRemoveConditionalOrder->ConditionalOrderID << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	//Ìõ¼þµ¥Ñ¡ÔñÏìÓ¦
	virtual void OnRspSelectConditionalOrder (CKSConditionalOrderRspResultField	*pSelectConditionalOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
	{
		printf ("hello select\n");
		if (pSelectConditionalOrder != NULL)
		{
			cout << "OnRspSelectConditionalOrder:"
				<< "ÓªÒµ²¿´úÂë:" << pSelectConditionalOrder->BrokerID
				<< "Í¶×ÊÕß´úÂë:" << pSelectConditionalOrder->InvestorID
				<< "Ìõ¼þµ¥±àºÅ:" << pSelectConditionalOrder->ConditionalOrderID << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	//Ó¯Ëðµ¥Â¼ÈëÏìÓ¦
	virtual void OnRspInsertProfitAndLossOrder (CKSProfitAndLossOrderOperResultField *pInsertProfitAndLossOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
	{
		printf ("hello profit and loss insert\n");
		if (pInsertProfitAndLossOrder != NULL)
		{
			cout << "OnRspInsertProfitAndLossOrder:"
				<< "ÓªÒµ²¿´úÂë:" << pInsertProfitAndLossOrder->BrokerID
				<< "Í¶×ÊÕß´úÂë:" << pInsertProfitAndLossOrder->InvestorID
				<< "Ö¹ËðÖ¹Ó¯µ¥ºÅ:" << pInsertProfitAndLossOrder->ProfitAndLossOrderID 
				<< "²Ù×÷Ô±:" << pInsertProfitAndLossOrder->UserID
				<< "Í¶×ÊÕßÃû³Æ:" << pInsertProfitAndLossOrder->InvestorName 
				<< "±¾µØ±¨µ¥±àºÅ:" << pInsertProfitAndLossOrder->OrderLocalID 
				<< "Ö¹Ëð¼Û:" << pInsertProfitAndLossOrder->StopLossPrice 
				<< "Ö¹Ó¯¼Û:" << pInsertProfitAndLossOrder->TakeProfitPrice 
				<< "Æ½²Ö·½Ê½:" << pInsertProfitAndLossOrder->CloseMode 
				<< "Æ½²Ö·´Ïò¼Ó¼õ¼ÛÎ»Êý:" << pInsertProfitAndLossOrder->Figures 
				<< "ÐÐÇé´¥·¢Ê±µÄ×îÐÂ¼Û:" << pInsertProfitAndLossOrder->LastPrice 
				<< "Éú³ÉÖ¹ËðÖ¹Ó¯µ¥Ê±¼ä:" << pInsertProfitAndLossOrder->ProfitAndLossOrderFormTime 
				<< "Éú³ÉÌõ¼þµ¥Ê±¼ä:" <<pInsertProfitAndLossOrder->ConditionalOrderFormTime 
				<< "Éú³ÉÎ¯ÍÐµ¥Ê±¼ä:" <<pInsertProfitAndLossOrder->OrderFormTime 
				<< "Ö¹ËðÖ¹Ó¯µ¥×´Ì¬:" << pInsertProfitAndLossOrder->ProfitAndLossOrderStatus 
				<< "Ìõ¼þµ¥±àºÅ:" <<pInsertProfitAndLossOrder->ConditionalOrderID 
				<< "½»Ò×Ëù´úÂë:" << pInsertProfitAndLossOrder->ExchangeID 
				<< "¿Í»§´úÂë:" << pInsertProfitAndLossOrder->ClientID 
				<< "ºÏÔ¼´úÂë:" << pInsertProfitAndLossOrder->InstrumentID 
				<< "¿ªÆ½±êÖ¾:" << pInsertProfitAndLossOrder->CombOffsetFlag 
				<< "Í¶»úÌ×±£±êÖ¾:" << pInsertProfitAndLossOrder->CombHedgeFlag 
				<< "ÂòÂô·½Ïò" << pInsertProfitAndLossOrder->Direction 
				<< "¼Û¸ñ" << pInsertProfitAndLossOrder->LimitPrice 
				<< "ÊýÁ¿" << pInsertProfitAndLossOrder->VolumeTotalOriginal 
				<< "Éú³ÉÓ¯Ëð¼ÛµÄ·½Ê½" <<pInsertProfitAndLossOrder->OffsetValue 
				<< "ÒµÎñµ¥Ôª" << pInsertProfitAndLossOrder->BusinessUnit 
				<< "Ìõ¼þµ¥´¥·¢¼Û¸ñÀàÐÍ" << pInsertProfitAndLossOrder->SpringType 
				<< "¸¡¶¯Ö¹Ëð¼Û" << pInsertProfitAndLossOrder->FloatLimitPrice 
				<< "¿ª²Ö³É½»¼Û¸ñ" << pInsertProfitAndLossOrder->OpenTradePrice << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

	}

	//Ó¯Ëðµ¥ÐÞ¸ÄÏìÓ¦
	virtual void OnRspModifyProfitAndLossOrder (CKSProfitAndLossOrderOperResultField *pModifyProfitAndLossOrder,	CThostFtdcRspInfoField * pRspInfo,	int nRequestID, bool bIsLast)
	{
		printf ("hello profit and loss modify\n");
		if (pModifyProfitAndLossOrder != NULL)
		{
			cout << "OnRspModifyProfitAndLossOrder:"
				<< "ÓªÒµ²¿´úÂë:" << pModifyProfitAndLossOrder->BrokerID
				<< "Í¶×ÊÕß´úÂë:" << pModifyProfitAndLossOrder->InvestorID
				<< "Ö¹ËðÖ¹Ó¯µ¥ºÅ:" << pModifyProfitAndLossOrder->ProfitAndLossOrderID 
				<< "²Ù×÷Ô±:" << pModifyProfitAndLossOrder->UserID 
				<< "Í¶×ÊÕßÃû³Æ:" << pModifyProfitAndLossOrder->InvestorName 
				<< "±¾µØ±¨µ¥±àºÅ:" << pModifyProfitAndLossOrder->OrderLocalID 
				<< "Ö¹Ëð¼Û:" << pModifyProfitAndLossOrder->StopLossPrice 
				<< "Ö¹Ó¯¼Û:" << pModifyProfitAndLossOrder->TakeProfitPrice 
				<< "Æ½²Ö·½Ê½:" << pModifyProfitAndLossOrder->CloseMode 
				<< "Æ½²Ö·´Ïò¼Ó¼õ¼ÛÎ»Êý:" << pModifyProfitAndLossOrder->Figures 
				<< "ÐÐÇé´¥·¢Ê±µÄ×îÐÂ¼Û:" << pModifyProfitAndLossOrder->LastPrice 
				<< "Éú³ÉÖ¹ËðÖ¹Ó¯µ¥Ê±¼ä:" << pModifyProfitAndLossOrder->ProfitAndLossOrderFormTime 
				<< "Éú³ÉÌõ¼þµ¥Ê±¼ä:" <<pModifyProfitAndLossOrder->ConditionalOrderFormTime 
				<< "Éú³ÉÎ¯ÍÐµ¥Ê±¼ä:" <<pModifyProfitAndLossOrder->OrderFormTime 
				<< "Ö¹ËðÖ¹Ó¯µ¥×´Ì¬:" << pModifyProfitAndLossOrder->ProfitAndLossOrderStatus 
				<< "Ìõ¼þµ¥±àºÅ:" <<pModifyProfitAndLossOrder->ConditionalOrderID 
				<< "½»Ò×Ëù´úÂë:" << pModifyProfitAndLossOrder->ExchangeID 
				<< "¿Í»§´úÂë:" << pModifyProfitAndLossOrder->ClientID 
				<< "ºÏÔ¼´úÂë:" << pModifyProfitAndLossOrder->InstrumentID 
				<< "¿ªÆ½±êÖ¾:" << pModifyProfitAndLossOrder->CombOffsetFlag 
				<< "Í¶»úÌ×±£±êÖ¾:" << pModifyProfitAndLossOrder->CombHedgeFlag 
				<< "ÂòÂô·½Ïò" << pModifyProfitAndLossOrder->Direction 
				<< "¼Û¸ñ" << pModifyProfitAndLossOrder->LimitPrice 
				<< "ÊýÁ¿" << pModifyProfitAndLossOrder->VolumeTotalOriginal 
				<< "Éú³ÉÓ¯Ëð¼ÛµÄ·½Ê½" <<pModifyProfitAndLossOrder->OffsetValue 
				<< "ÒµÎñµ¥Ôª" << pModifyProfitAndLossOrder->BusinessUnit 
				<< "Ìõ¼þµ¥´¥·¢¼Û¸ñÀàÐÍ" << pModifyProfitAndLossOrder->SpringType 
				<< "¸¡¶¯Ö¹Ëð¼Û" << pModifyProfitAndLossOrder->FloatLimitPrice 
				<< "¿ª²Ö³É½»¼Û¸ñ" << pModifyProfitAndLossOrder->OpenTradePrice << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	//Ó¯Ëðµ¥²éÑ¯ÏìÓ¦
	virtual void OnRspQueryProfitAndLossOrder (CKSProfitAndLossOrderOperResultField *pQueryProfitAndLossOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
	{
		printf ("hello profit and loss query\n");
		if (pQueryProfitAndLossOrder != NULL)
		{
			cout << "OnRspQueryProfitAndLossOrder:"
				<< "ÓªÒµ²¿´úÂë:" << pQueryProfitAndLossOrder->BrokerID
				<< "Í¶×ÊÕß´úÂë:" << pQueryProfitAndLossOrder->InvestorID
				<< "Ö¹ËðÖ¹Ó¯µ¥ºÅ:" << pQueryProfitAndLossOrder->ProfitAndLossOrderID
				<< "²Ù×÷Ô±:" << pQueryProfitAndLossOrder->UserID
				<< "Í¶×ÊÕßÃû³Æ:" << pQueryProfitAndLossOrder->InvestorName
				<< "±¾µØ±¨µ¥±àºÅ:" << pQueryProfitAndLossOrder->OrderLocalID
				<< "Ö¹Ëð¼Û:" << pQueryProfitAndLossOrder->StopLossPrice
				<< "Ö¹Ó¯¼Û:" << pQueryProfitAndLossOrder->TakeProfitPrice
				<< "Æ½²Ö·½Ê½:" << pQueryProfitAndLossOrder->CloseMode
				<< "Æ½²Ö·´Ïò¼Ó¼õ¼ÛÎ»Êý:" << pQueryProfitAndLossOrder->Figures
				<< "ÐÐÇé´¥·¢Ê±µÄ×îÐÂ¼Û:" << pQueryProfitAndLossOrder->LastPrice
				<< "Éú³ÉÖ¹ËðÖ¹Ó¯µ¥Ê±¼ä:" << pQueryProfitAndLossOrder->ProfitAndLossOrderFormTime 
				<< "Éú³ÉÌõ¼þµ¥Ê±¼ä:" <<pQueryProfitAndLossOrder->ConditionalOrderFormTime
				<< "Éú³ÉÎ¯ÍÐµ¥Ê±¼ä:" <<pQueryProfitAndLossOrder->OrderFormTime 
				<< "Ö¹ËðÖ¹Ó¯µ¥×´Ì¬:" << pQueryProfitAndLossOrder->ProfitAndLossOrderStatus 
				<< "Ìõ¼þµ¥±àºÅ:" <<pQueryProfitAndLossOrder->ConditionalOrderID 
				<< "½»Ò×Ëù´úÂë:" << pQueryProfitAndLossOrder->ExchangeID 
				<< "¿Í»§´úÂë:" << pQueryProfitAndLossOrder->ClientID 
				<< "ºÏÔ¼´úÂë:" << pQueryProfitAndLossOrder->InstrumentID 
				<< "¿ªÆ½±êÖ¾:" << pQueryProfitAndLossOrder->CombOffsetFlag 
				<< "Í¶»úÌ×±£±êÖ¾:" << pQueryProfitAndLossOrder->CombHedgeFlag 
				<< "ÂòÂô·½Ïò" << pQueryProfitAndLossOrder->Direction 
				<< "¼Û¸ñ" << pQueryProfitAndLossOrder->LimitPrice 
				<< "ÊýÁ¿" << pQueryProfitAndLossOrder->VolumeTotalOriginal 
				<< "Éú³ÉÓ¯Ëð¼ÛµÄ·½Ê½" <<pQueryProfitAndLossOrder->OffsetValue 
				<< "ÒµÎñµ¥Ôª" << pQueryProfitAndLossOrder->BusinessUnit 
				<< "Ìõ¼þµ¥´¥·¢¼Û¸ñÀàÐÍ" << pQueryProfitAndLossOrder->SpringType 
				<< "¸¡¶¯Ö¹Ëð¼Û" << pQueryProfitAndLossOrder->FloatLimitPrice 
				<< "¿ª²Ö³É½»¼Û¸ñ" << pQueryProfitAndLossOrder->OpenTradePrice << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	///Ö¹ËðÖ¹Ó¯µ¥É¾³ýÏìÓ¦
	virtual void  	OnRspRemoveProfitAndLossOrder (CKSProfitAndLossOrderRemoveField *pRemoveProfitAndLossOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
	{
		printf ("hello profit and loss delete\n");
		if (pRemoveProfitAndLossOrder != NULL)
		{
			cout << "OnRspRemoveProfitAndLossOrder:"
				<< "ÓªÒµ²¿´úÂë:" << pRemoveProfitAndLossOrder->BrokerID
				<< "Í¶×ÊÕß´úÂë" << pRemoveProfitAndLossOrder->InvestorID
				<< "Ö¹ËðÖ¹Ó¯µ¥ºÅ" << pRemoveProfitAndLossOrder->ProfitAndLossOrderID << endl;
		}
		printf ("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf ("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	///Ìõ¼þµ¥ÇëÇóÑ¡ÔñÍ¨Öª
	virtual void OnRtnCOSAskSelect (CKSCOSAskSelectField * pCOSAskSelect)
	{
		printf ("condition order select Notice\n");
		if (pCOSAskSelect != NULL)
		{
			cout << "OnRtnCOSAskSelect:"
				<< "ÓªÒµ²¿´úÂë:" << pCOSAskSelect->BrokerID
				<< "²Ù×÷Ô±:" << pCOSAskSelect->UserID
				<< "Í¶×ÊÕß´úÂë:" << pCOSAskSelect->InvestorID
				<< "ÐòºÅ" << pCOSAskSelect->SequenceNo
				<< "Ìõ¼þµ¥±àºÅ:" << pCOSAskSelect->ConditionalOrderID
				<< "±¸×¢:" << pCOSAskSelect->Memo
				<< "Ñ¡Ôñ·½Ê½:" << pCOSAskSelect->SelectType << endl;
		}
	}

	///Ìõ¼þµ¥×´Ì¬Í¨Öª
	virtual void OnRtnCOSStatus (CKSCOSStatusField * pCOSStatus)
	{
		printf ("condition order status notice\n");
		if (pCOSStatus != NULL)
		{
			cout << "OnRtnCOSStatus:"
				<< "ÓªÒµ²¿´úÂë:" << pCOSStatus->BrokerID
				<< "²Ù×÷Ô±:" << pCOSStatus->UserID
				<< "Í¶×ÊÕß´úÂë:" << pCOSStatus->InvestorID
				<< "ÐòºÅ:" << pCOSStatus->SequenceNo
				<< "Ìõ¼þµ¥±àºÅ:" << pCOSStatus->ConditionalOrderID
				<< "Ìõ¼þµ¥×´Ì¬:" << pCOSStatus->ConditionalOrderStatus
				<< "±¸×¢:" << pCOSStatus->Memo
				<< "±¾µØ±¨µ¥±àºÅ:" << pCOSStatus->OrderLocalID
				<< "½»Ò×Ëù´úÂë:" << pCOSStatus->ExchangeID
				<< "ºÏÔ¼´úÂë:" << pCOSStatus->InstrumentID
				<< "±¨µ¥×´Ì¬:" << pCOSStatus->OrderStatus
				<< "¿ªÆ½±êÖ¾:" << pCOSStatus->CombOffsetFlag
				<< "Í¶»úÌ×±£±êÖ¾:" << pCOSStatus->CombHedgeFlag
				<< "ÂòÂô·½Ïò:" << pCOSStatus->Direction
				<< "¼Û¸ñ:" << pCOSStatus->LimitPrice
				<< "ÊýÁ¿:" << pCOSStatus->VolumeTotalOriginal
				<< "½»Ò×ÈÕ:" << pCOSStatus->TradingDay
				<< "³·ÏúÓÃ»§:" << pCOSStatus->CancelUserID
				<< "³·ÏúÊ±¼ä:" << pCOSStatus->CancelTime
				<< "¿Í»§´úÂë:" << pCOSStatus->ClientID
				<< "ÒµÎñµ¥Ôª:" << pCOSStatus->BusinessUnit
				<< "±¨µ¥±àºÅ:" << pCOSStatus->OrderSysID
				<< "½ñ³É½»ÊýÁ¿:" << pCOSStatus->VolumeTraded
				<< "Ê£ÓàÊýÁ¿:" << pCOSStatus->VolumeTotal
				<< "Î¯ÍÐÊ±¼ä:" << pCOSStatus->InsertTime
				<< "¼¤»îÊ±¼ä:" << pCOSStatus->ActiveTime
				<< "³É½»¼Û¸ñ:" << pCOSStatus->TradePrice
				<< "»õ±Ò´úÂë:" << pCOSStatus->CurrencyID << endl;
		}
	}

	///Ö¹ËðÖ¹Ó¯µ¥×´Ì¬Í¨Öª
	virtual void OnRtnPLStatus (CKSPLStatusField * pPLStatus)
	{
		printf ("profit and loss order status notice\n");
		if (pPLStatus != NULL)
		{
			cout << "OnRtnPLStatus:"
				<< "ÓªÒµ²¿´úÂë:" << pPLStatus->BrokerID
				<< "²Ù×÷Ô±:" << pPLStatus->UserID
				<< "Í¶×ÊÕß´úÂë:" << pPLStatus->InvestorID
				<< "ÐòºÅ:" << pPLStatus->SequenceNo
				<< "Ö¹ËðÖ¹Ó¯µ¥±àºÅ:" << pPLStatus->ProfitAndLossOrderID
				<< "Ö¹ËðÌõ¼þµ¥±àºÅ:" << pPLStatus->StopLossOrderID
				<< "Ö¹Ó¯Ìõ¼þµ¥±àºÅ:" << pPLStatus->TakeProfitOrderID
				<< "Ó¯Ëðµ¥×´Ì¬:" << pPLStatus->ProfitAndLossOrderStatus
				<< "Ö¹Ëð¼Û:" << pPLStatus->StopLossPrice
				<< "Ö¹Ó¯¼Û:" << pPLStatus->TakeProfitPrice
				<< "Éú³ÉÓ¯Ëð¼ÛµÄ·½Ê½:" << pPLStatus->OffsetValue
				<< "¿ª²Ö³É½»¼Û¸ñ:" << pPLStatus->OpenTradePrice
				<< "±¸×¢:" << pPLStatus->Memo
				<< "±¾µØ±¨µ¥±àºÅ:" << pPLStatus->OrderLocalID
				<< "½»Ò×Ëù´úÂë:" << pPLStatus->ExchangeID
				<< "ºÏÔ¼´úÂë:" << pPLStatus->InstrumentID
				<< "±¨µ¥×´Ì¬:" << pPLStatus->OrderStatus
				<< "¿ªÆ½±êÖ¾:" << pPLStatus->CombOffsetFlag
				<< "Í¶»úÌ×±£±êÖ¾:" << pPLStatus->CombHedgeFlag
				<< "ÂòÂô·½Ïò:" << pPLStatus->Direction
				<< "¼Û¸ñ:" << pPLStatus->LimitPrice
				<< "ÊýÁ¿:" << pPLStatus->VolumeTotalOriginal
				<< "½»Ò×ÈÕ:" << pPLStatus->TradingDay
				<< "³·ÏúÓÃ»§:" << pPLStatus->CancelUserID
				<< "³·ÏúÊ±¼ä:" << pPLStatus->CancelTime
				<< "¿Í»§´úÂë:" << pPLStatus->ClientID
				<< "ÒµÎñµ¥Ôª:" << pPLStatus->BusinessUnit
				<< "±¨µ¥±àºÅ:" << pPLStatus->OrderSysID
				<< "½ñ³É½»ÊýÁ¿:" << pPLStatus->VolumeTraded
				<< "Ê£ÓàÊýÁ¿:" << pPLStatus->VolumeTotal
				<< "Î¯ÍÐÊ±¼ä:" << pPLStatus->InsertTime
				<< "¼¤»îÊ±¼ä:" << pPLStatus->ActiveTime
				<< "³É½»¼Û¸ñ:" << pPLStatus->TradePrice
				<< "»õ±Ò´úÂë:" << pPLStatus->CurrencyID << endl;
		}
	}
};

void FormatConditionalInsertData (CKSConditionalOrderInitInsert * ConditionOrder)
{
	strcpy (ConditionOrder->BrokerID, "6A89B428");
	strcpy (ConditionOrder->ExchangeID, "CFFEX");
	strcpy (ConditionOrder->InvestorID, "80002");
	strcpy (ConditionOrder->InstrumentID, "IF1303");
	ConditionOrder->Direction = '0';
	ConditionOrder->CombOffsetFlag = '0';
	ConditionOrder->CombHedgeFlag = '1';
	ConditionOrder->VolumeTotalOriginal = 2;
	ConditionOrder->LimitPrice = 200;
	ConditionOrder->OrderPriceType = '1';
	ConditionOrder->ConditionalType = '1';	//Ìõ¼þÀà±ð
	ConditionOrder->ConditionalPrice = 300;
	ConditionOrder->TriggeredTimes = 2;
	ConditionOrder->OrderType = '1';
	strcpy (ConditionOrder->ActiveTime, "142010");
	strcpy (ConditionOrder->InActiveTime, "162010");
	strcpy (ConditionOrder->CurrencyID, "RMB");

	return;

}

void FormatConditionalQueryData (CKSConditionalOrderQuery * Querycondition)
{
	strcpy (Querycondition->BrokerID, "6A89B428");
	strcpy (Querycondition->InvestorID, "80002");
	///²»´æÔÚµÄµ¥ºÅ
	//Querycondition->ConditionalOrderID  = 90;

	//ÒÑÖªµÄµ¥ºÅ 
	Querycondition->ConditionalOrderID  = 1;
	return;
}

void FormatConditionalPauseActiveData (CKSConditionalOrderStateAlter *Pausecondition)
{
	strcpy (Pausecondition->BrokerID, "6A89B428");
	strcpy (Pausecondition->InvestorID, "80002");
	Pausecondition->ConditionalOrderID = 18;
	Pausecondition->ConditionalOrderStateAlter = '0';

	return;
}

void FormatConditionalRemoveData (CKSConditionalOrderRemove * Removecondition)
{
	strcpy (Removecondition->BrokerID, "6A89B428");
	strcpy (Removecondition->InvestorID, "80002");
	Removecondition->ConditionalOrderID = 5;

	return;
}

void FormatConditionalModifyData (CKSConditionalOrderModify * Modifycondition)
{
	strcpy(Modifycondition->BrokerID,"6A89B428");
	strcpy(Modifycondition->InvestorID,"80002");
	Modifycondition->ConditionalOrderID = 16;
	Modifycondition->LimitPrice = 1000;
	Modifycondition->VolumeTotalOriginal = 9;
	strcpy (Modifycondition->ExchangeID, "CFFEX");
	strcpy (Modifycondition->InstrumentID, "IF1303");
	Modifycondition->Direction = '1';
	Modifycondition->CombOffsetFlag = '3';
	Modifycondition->CombHedgeFlag = '1';

	Modifycondition->OrderPriceType = '1';
	Modifycondition->ConditionalType = '1';	//Ìõ¼þÀà±ð
	Modifycondition->ConditionalPrice = 3000;
	Modifycondition->TriggeredTimes = 10;
	Modifycondition->OrderType = '1';
	return;
}

void FormatConditionalSelectData (CKSConditionalOrderSelect * Selectcondition)
{
	strcpy (Selectcondition->BrokerID, "6A89B428");
	strcpy (Selectcondition->InvestorID, "80002");
	Selectcondition->ConditionalOrderID = 2;
	Selectcondition->SelectResult = KSCOS_Select_AGAIN;
}

void FormatProfitAndLossInsertData (CKSProfitAndLossOrderInsert *ProfitAndLossOrder)
{
	strcpy (ProfitAndLossOrder->BrokerID, "6A89B428");
	strcpy (ProfitAndLossOrder->InvestorID, "80002");
	//strcpy (ProfitAndLossOrder->OrderLocalID, "8");
	//strcpy (ProfitAndLossOrder->OrderLocalID, "1");
	strcpy (ProfitAndLossOrder->OrderLocalID, "6");
	ProfitAndLossOrder->StopLossPrice = 2100;
	ProfitAndLossOrder->TakeProfitPrice = 2500;
	strcpy (ProfitAndLossOrder->ExchangeID, "CFFEX");
	strcpy (ProfitAndLossOrder->BusinessUnit, "cffex");
	ProfitAndLossOrder->TriggeredTimes = 2;
	ProfitAndLossOrder->CloseMode = '1';
	ProfitAndLossOrder->FiguresPrice = 0.2;
	ProfitAndLossOrder->OffsetValue = '0';
	ProfitAndLossOrder->SpringType = '1';
	strcpy (ProfitAndLossOrder->CurrencyID, "RMB");

	return;
}

void FormatProfitAndLossQueryData (CKSProfitAndLossOrderQuery *ProfitAndLossOrderQuery)
{
	///ÓªÒµ²¿´úÂë
	strcpy (ProfitAndLossOrderQuery->BrokerID, "6A89B428");
	///Í¶×ÊÕß´úÂë
	strcpy (ProfitAndLossOrderQuery->InvestorID, "80002");
	///Ö¹ËðÖ¹Ó¯µ¥ºÅ
	ProfitAndLossOrderQuery->ProfitAndLossOrderID = 12;
	///±¾µØ±¨µ¥±àºÅ
	strcpy (ProfitAndLossOrderQuery->OrderLocalID, "8");
	///½»Ò×Ëù´úÂë
	strcpy (ProfitAndLossOrderQuery->ExchangeID, "CFFEX");
	///ÒµÎñµ¥Ôª
	strcpy (ProfitAndLossOrderQuery->BusinessUnit, "cffex");
	return;
}

void FormatProfitAndLossRemoveData (CKSProfitAndLossOrderRemove *ProfitAndLossOrderRemove)
{
	///ÓªÒµ²¿´úÂë
	strcpy (ProfitAndLossOrderRemove->BrokerID, "6A89B428");
	///Í¶×ÊÕß´úÂë
	strcpy (ProfitAndLossOrderRemove->InvestorID, "80002");
	///Ö¹ËðÖ¹Ó¯µ¥ºÅ
	ProfitAndLossOrderRemove->ProfitAndLossOrderID = 2;
	///±¾µØ±¨µ¥±àºÅ
	strcpy (ProfitAndLossOrderRemove->OrderLocalID, "8");
	///½»Ò×Ëù´úÂë
	strcpy (ProfitAndLossOrderRemove->ExchangeID, "CFFEX");
	///ÒµÎñµ¥Ôª
	strcpy (ProfitAndLossOrderRemove->BusinessUnit, "cffex");

	return;
}

void FormatProfitAndLossModifyData (CKSProfitAndLossOrderModify *ProfitAndLossOrderModify)
{
	///ÓªÒµ²¿´úÂë
	strcpy (ProfitAndLossOrderModify->BrokerID, "6A89B428");
	///Í¶×ÊÕß´úÂë
	strcpy (ProfitAndLossOrderModify->InvestorID, "80002");
	///Ö¹ËðÖ¹Ó¯µ¥ºÅ
	ProfitAndLossOrderModify->ProfitAndLossOrderID =9 ;
	ProfitAndLossOrderModify->StopLossPrice = 2999;
	ProfitAndLossOrderModify->TakeProfitPrice = 3999;
	ProfitAndLossOrderModify->CloseMode = '1';
	ProfitAndLossOrderModify->FiguresPrice = 0.2;
	ProfitAndLossOrderModify->OffsetValue = '0';
	ProfitAndLossOrderModify->SpringType = '1';
	ProfitAndLossOrderModify->TriggeredTimes= 30;

	return;
}

//ÊµÀýÊýÁ¿
const int MAX_CONNECTION = 1;

int main(int argc, char* argv[])
{
	CThostFtdcTraderApi* pUserApi[MAX_CONNECTION] = {0};
	CTraderApiSample* pSpi[MAX_CONNECTION] = {0}; 
	CKSCosApi *pCosAPI = NULL;

	for (int i=0; i < MAX_CONNECTION; i++ )
	{
		// ´´½¨ÊµÀý
		// ÔËÐÐ´Ë³ÌÐòÇ°Çë½«»·¾³·þÎñÉÌÌá¹©µÄÊÚÈ¨ÎÄ¼þ·ÅÔÚ³ÌÐòÔËÐÐÄ¿Â¼
		pUserApi[i] = CThostFtdcTraderApi::CreateFtdcTraderApi();

		// ´´½¨spiÊµÀý
		// ÔËÐÐ´Ë³ÌÐòÇ°ÇëÐÞ¸ÄBrokerID ÓÃ»§Ãû¼°ÃÜÂë
		pSpi[i] = new CTraderApiSample(pUserApi[i], "31000853", "80002", "123456", "IF1506");

		CCosHandler CosSpiTest;			//²úÉúÒ»¸öÌõ¼þµ¥ÏìÓ¦µÄÊµÀý

		// ×¢²áspiÊµÀý
		pUserApi[i]->RegisterSpi(pSpi[i]);

		//¶©ÔÄË½ÓÐÁ÷
		pUserApi[i]->SubscribePrivateTopic(THOST_TERT_RESUME);

		//¶©ÔÄ¹«ÓÐÁ÷
		pUserApi[i]->SubscribePublicTopic(THOST_TERT_QUICK);

		// ×¢²áÇ°ÖÃ»ú	
		// ÔËÐÐ´Ë³ÌÐòÇ°ÇëÐÞ¸Ä IP¶Ë¿Ú
		pUserApi[i]->RegisterFront("tcp://10.253.117.107:13163");
		//pUserApi[i]->RegisterNameServer("tcp://10.253.117.110:11000");
		//CThostFtdcFensUserInfoField FensUserInfo = {0};
		//strncpy(FensUserInfo.UserID, "201301", sizeof(FensUserInfo.UserID)-1);
		//FensUserInfo.LoginMode = 'E';
		//pUserApi[i]->RegisterFensUserInfo(&FensUserInfo);
		// ³õÊ¼»¯
		pUserApi[i]->Init();
		getchar();

		pCosAPI = (CKSCosApi *) pUserApi[i]->LoadExtApi(&CosSpiTest,KS_COS_API);	//×¢²áÌõ¼þµ¥ÊµÀý
		if( NULL == pCosAPI )
		{
			printf("register conditional order instance failed\n");
		}
		//Ìõ¼þµ¥ÏÂµ¥
		CKSConditionalOrderInitInsert ConditionOrder;
		memset (&ConditionOrder, 0, sizeof (ConditionOrder));
		FormatConditionalInsertData (&ConditionOrder);

		//²éÑ¯Ìõ¼þµ¥ÊµÀý 
		CKSConditionalOrderQuery Querycondition;
		memset (&Querycondition, 0, sizeof (CKSConditionalOrderQuery));
		FormatConditionalQueryData (&Querycondition);

		pCosAPI->ReqInitInsertConditionalOrder(&ConditionOrder, pSpi[i]->m_nRequestID++);
		pCosAPI->ReqQueryConditionalOrder (&Querycondition, pSpi[i]->m_nRequestID++);
	}

	printf ("\npress return to quit...\n");

	getchar();
	for (int i=0; i < MAX_CONNECTION; i++ )
	{
		// release the API instance
		pUserApi[i]->Release();

		// delete pSpi
		delete pSpi[i];
	}

	return 0;
}


