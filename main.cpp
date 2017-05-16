/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20-02-2015 10:13:39 WET
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <iostream>
#include <string.h>
#include <pqxx/pqxx> 
#include "pqxx/nontransaction"
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include <time.h>
#include <regex.h>


/* #####   TYPE DEFINITIONS  -  LOCAL TO THIS SOURCE FILE   ######################### */
// static connection C;
// static connection C("dbname=koncepto user=postgres hostaddr=127.0.0.1 port=5432");

// Change these to suit your needs.
static const char conn_str[] =
  "user='postgres' password='' host='127.0.0.1' port='5432' dbname='koncepto'";
// static const char channel_name[] = "job_queue";
// static const char app_name[] = "\"pqxx-listener-test\"";

pqxx::connection C(conn_str);
pqxx::nontransaction N(C);

// nontransaction N(C);


/* #####   DATA TYPES  -  LOCAL TO THIS SOURCE FILE   ############################### */
using namespace std;
using namespace pqxx;

/* #####   VARIABLES  -  LOCAL TO THIS SOURCE FILE   ################################ */
xmlTextWriterPtr writer;
int rc=0;

char *year, *month, *file, *database ;

/* #####   CONSTANTS  -  LOCAL TO THIS SOURCE FILE   ################################ */


char SALE[]="2";


/* #####   MACROS  -  LOCAL TO THIS SOURCE FILE   ################################### */
#define MY_ENCODING "Windows-1252"



/* #####   PROTOTYPES  -  LOCAL TO THIS SOURCE FILE   ############################### */
void OpenPSQL (void);
void LoadHeader (void);
void ClosePSQL (void);
void LoadCustomers (void);
void LoadProducts (void);
void LoadTaxes (void);
void LoadTotals (void);
void LoadDocuments (void);

	
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: main 
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char* argv[])
	{
	// Check the number of parameters
	if (argc < 3) {
		// Tell the user how to run the program
		std::cerr << "Usage: " << argv[0] << " year month file database" << std::endl;
		/* "Usage messages" are a conventional way of telling the user
		 * how to run a program if they enter the command incorrectly.
		 */
		std::cerr << "Ignoring.."<< std::endl;
		// return 1;
	}

	year=argv[1];
	month=argv[2];
	file=argv[3];
	database=argv[4];

	// Print the user's name:
	// std::cout << argv[0] << "says hello, " << argv[1] << "!" << std::endl;
	 std::cout << "YEAR= " << year << "; MONTH= " << month << "; Database: " << database << std::endl;
//	return 0;
 


	OpenPSQL();

	//  Open File 
	printf("Starting: %d\n",rc);
	/* writer = xmlNewTextWriterFilename("teste.xml",0); */ 
	writer = xmlNewTextWriterFilename(file,0);
	xmlTextWriterSetIndent(writer, 4);

	rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
	rc = xmlTextWriterStartElement(writer, BAD_CAST "AuditFile");
	xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "urn:OECD:StandardAuditFile-Tax:PT_1.03_01");
	//e xmlns="urn:OECD:StandardAuditFile-Tax:PT_1.03_01"

	printf("A preencher cabecalho\n");
	LoadHeader ();

	rc = xmlTextWriterStartElement(writer, BAD_CAST "MasterFiles");


	printf("A preencher clientes\n");
	LoadCustomers ();

	printf("A preencher produtos\n");
	LoadProducts ();

	LoadTaxes ();
	rc = xmlTextWriterEndElement(writer); // MasterFiles

 	xmlTextWriterStartElement(writer, BAD_CAST "SourceDocuments");
 	xmlTextWriterStartElement(writer, BAD_CAST "SalesInvoices");
	LoadTotals ();
	LoadDocuments ();
	rc = xmlTextWriterEndElement(writer); 
	rc = xmlTextWriterEndElement(writer); 
	

	rc = xmlTextWriterEndElement(writer); // AuditFile


	rc = xmlTextWriterEndElement(writer); //SourceDocuments 

	ClosePSQL ();


	xmlFreeTextWriter(writer);




	// xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceDate", BAD_CAST c[2].as<string>().c_str());

	// rc = xmlTextWriterStartElement(writer, BAD_CAST "DocumentTotals");
	// xmlTextWriterWriteElement(writer, BAD_CAST "GrossTotal", BAD_CAST c[3].as<string>().c_str());
	// rc = xmlTextWriterEndElement(writer);//DocumentStatus

	// rc = xmlTextWriterEndElement(writer);	



	// rc = xmlTextWriterStartElement(writer, BAD_CAST "Customer");
	// rc = xmlTextWriterEndElement(writer);





	/*
	   rc = xmlTextWriterStartElement(writer, BAD_CAST "Product");
	   rc = xmlTextWriterEndElement(writer);
	   rc = xmlTextWriterStartElement(writer, BAD_CAST "TaxTable");
	   rc = xmlTextWriterEndElement(writer);

	   rc = xmlTextWriterEndElement(writer);

	   rc = xmlTextWriterStartElement(writer, BAD_CAST "SourceDocuments");
	   rc = xmlTextWriterStartElement(writer, BAD_CAST "SalesInvoices");
	 */
	//	char sql[] = "SELECT doc_number, doc_type, to_char(doc_date, 'YYYY-MM-DD\"T\"HH24:MI:SS'), info_client_total,doc_hash from documents WHERE doc_type IN (2)";
	//	nontransaction N(C);
	//	result R( N.exec( sql ));

	/*	for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
		rc = xmlTextWriterStartElement(writer, BAD_CAST "Invoice");

		xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceNo", BAD_CAST c[0].as<string>().c_str());

		rc = xmlTextWriterStartElement(writer, BAD_CAST "DocumentStatus");
		xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceStatusDate", BAD_CAST c[2].as<string>().c_str());
		rc = xmlTextWriterEndElement(writer);//DocumentStatus

		xmlTextWriterWriteElement(writer, BAD_CAST "Hash", BAD_CAST c[4].as<string>().c_str());

		xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceDate", BAD_CAST c[2].as<string>().c_str());

		rc = xmlTextWriterStartElement(writer, BAD_CAST "DocumentTotals");
		xmlTextWriterWriteElement(writer, BAD_CAST "GrossTotal", BAD_CAST c[3].as<string>().c_str());
		rc = xmlTextWriterEndElement(writer);//DocumentStatus

		rc = xmlTextWriterEndElement(writer);//Invoice

		}

	 */	

	/* 	SELECT 
	 *         d.doc_number
	 *         , c.client_account as antes
	 *         , cl.id as depois
	 *         , ent.id as entityID
	 *         ,ent.tax_number
	 * 	FROM documents d
	 * 	INNER JOIN cashflow c
	 * 		ON d.id=c.reference_document_id AND c.doc_type = 0 AND c.payment_method = 7
	 * 	INNER JOIN clientcard cc
	 * 		ON c.client_account=cc.id
	 * 	INNER JOIN client cl
	 * 		ON cc.clientid=cl.id
	 * 	INNER JOIN entity ent
	 * 		ON cl.entityid=ent.id
	 * 	WHERE d.doc_number='VB152761'; 
	 */

}




void OpenPSQL (void)
{
	/*-----------------------------------------------------------------------------
	 *   Open PSQL Connection 
	 *-----------------------------------------------------------------------------*/
	//try{
		//connection C("dbname=koncepto user=postgres hostaddr=127.0.0.1 port=5432");
		// connection C("dbname=koncepto user=postgres hostaddr=127.0.0.1 port=5432");
		// connection C("dbname=koncepto user=postgres hostaddr=127.0.0.1 port=5432");
		// pqxx::connection db(conn_str);
		 // pqxx::connection C(conn_str);
		
	if (C.is_open()) {
		// 	cout << "Opened database successfully: \n" << C.dbname() << endl;
		} else {
			cout << "Can't open database" << endl;
			 // return 1;
		}
	/*}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}*/
}

void ClosePSQL (void)
{
	/*-----------------------------------------------------------------------------
	 *   Close PSQL Connection 
	 *-----------------------------------------------------------------------------*/
	C.disconnect ();

}


void LoadHeader (void )
{
	xmlTextReaderPtr reader = xmlReaderForFile("../config/data.xml", NULL, 0);
	
	if (reader == NULL){
		fprintf(stderr, "Ficheiro de configuarcao em falta!!\n");
		exit(1);
	}
	
	const xmlChar *name, *value;
	char CompanyID[15],CompanyName[250],BusinessName[250],AddressDetail[101],PostalCode[20],City[51];
	int ret;
	ret = xmlTextReaderRead(reader);
	while (ret == 1) {
		name = xmlTextReaderConstName(reader);
		if (name == NULL)
			name = BAD_CAST "--";
		
		if(strcmp("FiscalNumber", (const char *)name)==0){
			ret = xmlTextReaderRead(reader);
			sprintf(CompanyID,"%s",xmlTextReaderConstValue(reader));
			ret = xmlTextReaderRead(reader);
		}
		
		if(strcmp("SocialName", (const char *)name)==0){
			ret = xmlTextReaderRead(reader);
			sprintf(CompanyName,"%s",xmlTextReaderConstValue(reader));
			ret = xmlTextReaderRead(reader);
		}
		
		if(strcmp("ComercialName", (const char *)name)==0){
			ret = xmlTextReaderRead(reader);
			sprintf(BusinessName,"%s",xmlTextReaderConstValue(reader));
			ret = xmlTextReaderRead(reader);
		}
		if(strcmp("Address", (const char *)name)==0){
			ret = xmlTextReaderRead(reader);
			sprintf(AddressDetail,"%s",xmlTextReaderConstValue(reader));
			ret = xmlTextReaderRead(reader);
		}
		if(strcmp("ZipCode", (const char *)name)==0){
			ret = xmlTextReaderRead(reader);
			sprintf(PostalCode,"%s",xmlTextReaderConstValue(reader));
			ret = xmlTextReaderRead(reader);
		}
		if(strcmp("City", (const char *)name)==0){
			ret = xmlTextReaderRead(reader);
			sprintf(City,"%s",xmlTextReaderConstValue(reader));
			ret = xmlTextReaderRead(reader);
		}
		
		//value = xmlTextReaderConstValue(reader);
		
		
		
		ret = xmlTextReaderRead(reader);
	}
	
	
	// bool bHasMulti;
	//ini.GetValue(_T("INFO"), _T("ComercialName"), 0, &bHasMulti);
	/*-----------------------------------------------------------------------------
	 * Header 
	 *   All header info MUST be available throught koncepto.ini file
	 *   based on license file
	 *-----------------------------------------------------------------------------*/
	rc = xmlTextWriterStartElement(writer, BAD_CAST "Header");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "AuditFileVersion", BAD_CAST "1.03_01");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "CompanyID", BAD_CAST CompanyID);
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "TaxRegistrationNumber", BAD_CAST CompanyID);
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "TaxAccountingBasis", BAD_CAST "F");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "CompanyName", BAD_CAST CompanyName);
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "BusinessName", BAD_CAST BusinessName);
	
	xmlTextWriterStartElement(writer, BAD_CAST "CompanyAddress");
		rc = xmlTextWriterWriteElement(writer, BAD_CAST "AddressDetail", BAD_CAST AddressDetail);
		rc = xmlTextWriterWriteElement(writer, BAD_CAST "City", BAD_CAST City);
		rc = xmlTextWriterWriteElement(writer, BAD_CAST "PostalCode", BAD_CAST PostalCode);
		rc = xmlTextWriterWriteElement(writer, BAD_CAST "Country", BAD_CAST "PT");
	xmlTextWriterEndElement(writer);
	
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "FiscalYear", BAD_CAST year);
	
	
	struct tm my_time;
	my_time.tm_year=atoi(year)-1900; // = year 2012
        my_time.tm_mon=atoi(month)-1;    // = 10th month
        my_time.tm_mday=1;   // = 9th day
	my_time.tm_hour=0;   // = 8 hours
	my_time.tm_min=0;   // = 10 minutes
	my_time.tm_sec=0;    // = 20 secs
	my_time.tm_isdst=0;    // = 20 secs
	mktime(&my_time);
	
	//time = &my_time;
	/*time->tm_year = atoi(year) - 1900;
	time->tm_mon = atoi(month)-1;
	time->tm_mday = 1;
	mktime(time);
	//mktime(time);*/
	
	char buff[70];
	//strftime(buff, sizeof buff, "%Y-%m-%d %z", localtime(&t));
	strftime(buff, sizeof buff, "%Y-%m-%d", &my_time);
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "StartDate", BAD_CAST buff);
	my_time.tm_mon = atoi(month);
	my_time.tm_mday = 0;
	mktime(&my_time);
	strftime(buff, sizeof buff, "%Y-%m-%d", &my_time);
	
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "EndDate", BAD_CAST buff);
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "CurrencyCode", BAD_CAST "EUR");
	
	time_t rawtime;
	time( &rawtime );
	struct tm *info;
	info = localtime( &rawtime );
	strftime(buff, sizeof buff, "%Y-%m-%d", info);
	
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "DateCreated", BAD_CAST buff);
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "TaxEntity", BAD_CAST "Global");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "ProductCompanyTaxID", BAD_CAST "508476879");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "SoftwareCertificateNumber", BAD_CAST "1183");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "ProductID", BAD_CAST "Koncepto/SINDESI SISTEMAS E SOLUCOES INFORMATICAS LDA");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "ProductVersion", BAD_CAST "4.3.4");
	rc = xmlTextWriterEndElement(writer);


}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  LoadCustomers
 *  Description:  
 * =====================================================================================
 */
void LoadCustomers (void)
{
	/*-----------------------------------------------------------------------------
	 *  Customer List
	 *-----------------------------------------------------------------------------*/
	char sql[]="SELECT 								"\
		" 	c.id AS client_id, 						"\
		"	CASE  								"\
		"	WHEN LENGTH(COALESCE(TRIM(e.name), 'Desconhecido')) = 0 	"\
		"		THEN 'Desconhecido'					"\
		"	ELSE 								"\
		"		COALESCE(TRIM(e.name), 'Desconhecido')			"\
		"	END AS client_name, 						"\
		"	CASE  								"\
		"	WHEN LENGTH(COALESCE(TRIM(e.address), 'Desconhecido')) = 0 	"\
		"		THEN 'Desconhecido'					"\
		"		ELSE COALESCE(TRIM(e.address), 'Desconhecido')		"\
		"	END AS client_address, 						"\
		"	COALESCE(TRIM(e.postal_code), '0000-000') AS client_zipcode,		"\
		"	COALESCE(TRIM(e.phone), '') AS client_phone,			"\
		"	COALESCE(TRIM(clientcards.name), 				"\
		"	COALESCE(TRIM(e.tax_number), '999999990')) AS client_taxnumber	"\
		"	FROM 								"\
		"	client c        LEFT OUTER JOIN         (               	"\
		"	SELECT                  cc.clientid AS clientid, max(cc.name) 	"\
		"	AS NAME           FROM                    clientcard cc  	"\
		"	GROUP BY                        cc.clientid     ) 		"\
		"	clientcards ON                clientcards .clientid = c.id    	"\
		"	LEFT OUTER JOIN entity e 					"\
		"	ON             e.id = c.entityid ORDER BY      c.id		";



	result R( N.exec( sql ));

	/*      <Customer>
	 *       <CustomerID>Consumidor Final</CustomerID>
	 *       <AccountID>Desconhecido</AccountID>
	 *       <CustomerTaxID>999999990</CustomerTaxID>
	 *       <CompanyName>Consumidor Final</CompanyName>
	 *       <BillingAddress>
	 *         <AddressDetail>Desconhecido</AddressDetail>
	 *         <City>Desconhecido</City>
	 *         <PostalCode>Desconhecido</PostalCode>
	 *         <Country>Desconhecido</Country>
	 *       </BillingAddress>
	 *       <SelfBillingIndicator>0</SelfBillingIndicator>
	 *     </Customer> 
	 */
	 
	
	//rc = xmlTextWriterStartElement(writer, BAD_CAST "Customer");
	for (result::const_iterator c = R.begin(); c != R.end(); ++c) {

		xmlTextWriterStartElement(writer, BAD_CAST "Customer");
		xmlTextWriterWriteElement(writer, BAD_CAST "CustomerID", BAD_CAST c[0].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "AccountID", BAD_CAST "Desconhecido");
		xmlTextWriterWriteElement(writer, BAD_CAST "CustomerTaxID", BAD_CAST c[5].as<string>().c_str());		
		xmlTextWriterWriteElement(writer, BAD_CAST "CompanyName", BAD_CAST c[1].as<string>().c_str());
		xmlTextWriterStartElement(writer, BAD_CAST "BillingAddress");
		xmlTextWriterWriteElement(writer, BAD_CAST "AddressDetail", BAD_CAST c[2].as<string>().c_str());
		
		xmlTextWriterWriteElement(writer, BAD_CAST "City", BAD_CAST "Desconhecido");
		
		if(strcmp(c[3].as<string>().c_str(),"")==0)
			xmlTextWriterWriteElement(writer, BAD_CAST "PostalCode", BAD_CAST "0000-000");
		else
			xmlTextWriterWriteElement(writer, BAD_CAST "PostalCode", BAD_CAST c[3].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "Country", BAD_CAST "PT");
		xmlTextWriterEndElement(writer);	//BillingAddress
		xmlTextWriterWriteElement(writer, BAD_CAST "SelfBillingIndicator", BAD_CAST "0");
		xmlTextWriterEndElement(writer);//Customer
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  LoadProducts
 *  Description:  
 * =====================================================================================
 */

void LoadProducts (void)
{

	/*-----------------------------------------------------------------------------
	 *  Product List
	 *-----------------------------------------------------------------------------*/
	
	/* 	<Product>
	 * 		<ProductType>P</ProductType>
	 * 		<ProductCode>5</ProductCode>
	 * 		<ProductDescription>garoto</ProductDescription>
	 * 		<ProductNumberCode>5</ProductNumberCode>
	 * 	</Product>
	 */
	

	char products[]="SELECT id, description FROM products WHERE product_type NOT IN (-5, -4, 6);";
	
	
//	printf("Aqui\n");
	result PR( N.exec( products ));
	for (result::const_iterator c = PR.begin(); c != PR.end(); ++c) {

		xmlTextWriterStartElement(writer, BAD_CAST "Product");
		xmlTextWriterWriteElement(writer, BAD_CAST "ProductType", BAD_CAST "P");
		xmlTextWriterWriteElement(writer, BAD_CAST "ProductCode", BAD_CAST c[0].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "ProductDescription", BAD_CAST c[1].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "ProductNumberCode", BAD_CAST c[0].as<string>().c_str());
		xmlTextWriterEndElement(writer);  //Product
	}

	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  LoadTaxes
 *  Description:  
 * =====================================================================================
 */
void LoadTaxes (void)
{
	/* 	
	 * 	    <TaxTable>
	 *       <TaxTableEntry>
	 *         <TaxType>IVA</TaxType>
	 *         <TaxCountryRegion>PT</TaxCountryRegion>
	 *         <TaxCode>ISE</TaxCode>
	 *         <Description>IVA 0%</Description>
	 *         <TaxPercentage>0.00</TaxPercentage>
	 *       </TaxTableEntry>
	 *       <TaxTableEntry>
	 *         <TaxType>IVA</TaxType>
	 *         <TaxCountryRegion>PT</TaxCountryRegion>
	 *         <TaxCode>NOR</TaxCode>
	 *         <Description>IVA 23%</Description>
	 *         <TaxPercentage>23.00</TaxPercentage>
	 *       </TaxTableEntry>
	 *     </TaxTable>
	 *   </MasterFiles>
	 * 	
	 */
	
		char taxes[]="SELECT DISTINCT					"\
        "       CAST('IVA' AS CHAR(3)) AS TaxType, 				"\
        "       CAST('PT' AS CHAR(2)) AS TaxCountryRegion,   			"\
	"       CAST(  								"\
	"		CASE    						"\
	"                  WHEN (p.Tax1/100) = 0 THEN 'ISE'   			"\
	"		   WHEN (p.Tax1/100) IN (5, 6, 7) THEN 'RED'  		"\
	"		   WHEN (p.Tax1/100) IN (11, 12, 13) THEN 'INT'   	"\
	"		   WHEN (p.Tax1/100) IN (19, 20, 21, 22, 23) THEN 'NOR' "\
	"		ELSE 'OUT'   						"\
	"	END AS CHAR(3)  ) 						"\
	"	AS TaxCode, CAST(('IVA ' || (p.Tax1/100) || '%')  		"\
	"	AS VARCHAR(255)) AS Description,  				"\
	"	CAST((p.Tax1/100) AS DECIMAL(18,2)) AS TaxPercentage  		"\
	"	FROM  products p ORDER BY   5; 					";
	
		
	xmlTextWriterStartElement(writer, BAD_CAST "TaxTable");
	result TAX( N.exec( taxes ));
	for (result::const_iterator c = TAX.begin(); c != TAX.end(); ++c) {

		xmlTextWriterStartElement(writer, BAD_CAST "TaxTableEntry");
		xmlTextWriterWriteElement(writer, BAD_CAST "TaxType",  BAD_CAST c[0].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "TaxCountryRegion", BAD_CAST c[1].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "TaxCode", BAD_CAST c[2].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "Description", BAD_CAST c[3].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "TaxPercentage", BAD_CAST c[4].as<string>().c_str());
		xmlTextWriterEndElement(writer);  //TaxTableEntry
	}

	xmlTextWriterEndElement(writer);  //TaxTable

	
	
	//rc = xmlTextWriterEndElement(writer);//SalesInvoices	

}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  LoadTotals
 *  Description:  
 * =====================================================================================
 */
void LoadTotals (void)
{
	/*   <SourceDocuments>
	 *     <SalesInvoices>
	 *       <NumberOfEntries>287</NumberOfEntries>
	 *       <TotalDebit>85.37</TotalDebit>
	 *       <TotalCredit>7098.28</TotalCredit>
	 */

 
	char query[2000];
  	char transactions[]="SELECT 						"\
        " COUNT(d.id) as total,      						"\
	"       COALESCE( SUM(        						"\
	"                CASE    						"\
	"                        WHEN d.doc_type = 2 				"\
	"                                THEN d.total_without_tax		"\
	"                        ELSE \'0\'					"\
	"                END							"\
	"        ),0)   AS credit,     						"\
	"	COALESCE (SUM(							"\
	"                CASE 							"\
	"                        WHEN d.doc_type = 9 				"\
	"                                THEN d.total_without_tax		"\
	"                        ELSE \'0\' 					"\
	"                END							"\
	"        ),0) AS debit							"\
	"FROM 									"\
        "	documents d							"\
	"	WHERE								"\
        "	EXTRACT ('year' FROM d.doc_date) = '%s' 			"\
        "	AND    EXTRACT('month' FROM d.doc_date) = '%s'			"\
        "	AND    d.doc_type IN (2, 9); 					";
	
	sprintf(query,transactions,year,month);
	
	
	float  Total = 0;
	char buf[20];
  
  	result TRANS( N.exec( query ));
  	for (result::const_iterator c = TRANS.begin(); c != TRANS.end(); ++c) {
		xmlTextWriterWriteElement(writer, BAD_CAST "NumberOfEntries", BAD_CAST c[0].as<string>().c_str());
		
		
		Total = c[2].as<int>()/100.0;
		sprintf(buf,"%4.2f",Total);
   		xmlTextWriterWriteElement(writer, BAD_CAST "TotalDebit", BAD_CAST buf);
		Total = c[1].as<int>()/100.0;
		sprintf(buf,"%4.2f",Total);
   		xmlTextWriterWriteElement(writer, BAD_CAST "TotalCredit", BAD_CAST buf);
  	}
}



 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  LoadDocuments
 *  Description:  
 * =====================================================================================
 */
void LoadDocuments (void)
{ 
	/*       <Invoice>
	 *         <InvoiceNo>VD NOLV/17981</InvoiceNo>
	 *         <DocumentStatus>
	 *           <InvoiceStatus>N</InvoiceStatus>
	 *           <InvoiceStatusDate>2014-11-01T11:42:36</InvoiceStatusDate>
	 *           <SourceID>1</SourceID>
	 *           <SourceBilling>P</SourceBilling>
	 *         </DocumentStatus>
	 *         <Hash>Br+1vV9sscWXGVpb9bIhLURHKnM3rxf+Fsx9I3K7L7rKKOSEsSfR79u+CQIbNgdqE0uMKWgEXH6tiplQiypB/mfk0aB+aS4g4+7VNEeRaHiIKlNcBSmWD9BwUE8bym0heKLyv5WkX7KBu6I6R/ANfgpVNflCTD9kHyXH3on3hEU=</Hash>
	 *         <HashControl>1</HashControl>
	 *         <Period>11</Period>
	 *         <InvoiceDate>2014-11-01</InvoiceDate>
	 *         <InvoiceType>FT</InvoiceType>
	 *         <SpecialRegimes>
	 *           <SelfBillingIndicator>0</SelfBillingIndicator>
	 *           <CashVATSchemeIndicator>0</CashVATSchemeIndicator>
	 *           <ThirdPartiesBillingIndicator>0</ThirdPartiesBillingIndicator>
	 *         </SpecialRegimes>
	 *         <SourceID>1</SourceID>
	 *         <SystemEntryDate>2014-11-01T11:42:36</SystemEntryDate>
	 *         <CustomerID>Consumidor Final</CustomerID>
	 *         <Line>
	 *           <LineNumber>1</LineNumber>
	 *           <ProductCode>13</ProductCode>
	 *           <ProductDescription>Manutencao Cor</ProductDescription>
	 *           <Quantity>1</Quantity>
	 *           <UnitOfMeasure>un</UnitOfMeasure>
	 *           <UnitPrice>32.52</UnitPrice>
	 *           <TaxPointDate>2014-11-01</TaxPointDate>
	 *           <Description>Manutencao Cor</Description>
	 *           <CreditAmount>32.52</CreditAmount>
	 *           <Tax>
	 *             <TaxType>IVA</TaxType>
	 *             <TaxCountryRegion>PT</TaxCountryRegion>
	 *             <TaxCode>NOR</TaxCode>
	 *             <TaxPercentage>23</TaxPercentage>
	 *           </Tax>
	 *         </Line>
	 *         <DocumentTotals>
	 *           <TaxPayable>7.48</TaxPayable>
	 *           <NetTotal>32.52</NetTotal>
	 *           <GrossTotal>40.00</GrossTotal>
	 *         </DocumentTotals>
	 *       </Invoice>
	 */
		
	char query[2000];
	char documents[]="SELECT d.id, \
	d.doc_hash,\
	d.doc_date,\
	d.doc_type,\
	d.total_tax1, \
	d.total_without_tax,\
	d.total_with_tax,\
	NULL AS PaymentMethod,\
	d.doc_number,\
	EXTRACT('month' FROM d.doc_date) AS period,\
	CAST('N' AS VARCHAR) AS InvoiceStatus,\
	COALESCE(CAST(d.doc_hash_control AS VARCHAR), ''),\
	COALESCE(cdoc.id, 0) AS client_account,\
	-- detalhes \n\
	dd.product_id,\
	dd.product_name,\
	dd.quantity,\
	u.name,\
	dd.price_without_tax,\
	dd.price_without_tax,\
	dd.tax1,\
	COALESCE(dvc.doc_number, dvd.doc_number, '-1') AS Reference,\
	COALESCE(CAST(dvc.doc_date AS DATE), CAST(dvd.doc_date AS DATE)) AS ReferenceDate\
	FROM documents d\
	LEFT JOIN (\
		SELECT c.reference_document_id, cl.id\
		FROM documents d\
		INNER JOIN cashflow c\
		       ON d.id=c.reference_document_id AND c.doc_type = 0 AND c.payment_method = 7\
		INNER JOIN clientcard cc\
		       ON c.client_account=cc.id\
		INNER JOIN client cl\
		       ON cc.clientid=cl.id\
		GROUP BY c.reference_document_id, cl.id\
	) cdoc\
		ON d.id=cdoc.reference_document_id\
	LEFT JOIN documentdetails dd\
		ON d.id=dd.document_id\
	LEFT OUTER JOIN units u\
		ON dd.unit = u.id\
	LEFT OUTER JOIN documents dvc\
		ON dvc.id = d.reference_doc_id\
			AND dvc.doc_type=3 \
	LEFT OUTER JOIN documents dvd\
		ON dvd.id = d.reference_doc_id\
			AND dvd.doc_type = 2\
	WHERE EXTRACT('year' FROM d.doc_date) = '%s'\
		AND EXTRACT('month' FROM d.doc_date) = '%s'\
		AND d.doc_type IN (2, 9)\
		ORDER BY d.id,dd.id";
	/* 	ORDER BY d.id  */

	sprintf(query,documents,year,month);


/*   id   |    doc_hash   |    doc_date |  doc_type | total_tax1 | total_without_tax | total_with_tax | paymentmethod | doc_number | period | invoicestatus | coalesce | client_account 
 366577 | 4mw4dUns= | 2014-12-27 23:16:02.048 |        2 |       2002 |              8708 |          10710 |               | VB152761   |     12 | N             | 1        |           3390
*/
	
	int    status;
	regex_t    re;
	regmatch_t pm[3];
	if (regcomp(&re, "([a-zA-Z]*)([0-9]*)", REG_EXTENDED) != 0){
		printf("Não compilou\n");
		exit(1);
	}
	
	float TaxPayable = 0;
	float NetTotal = 0;
	float GrossTotal = 0;


	int lastID = 0;
	int ID = 0;
	int lineNum = 0;
	char buf[150];
	char buf2[150];
	float Total = 0;
	int IVA = 0;
	bool nc=false;
	tm mtm;
	const char *pt;
  	result DOCS( N.exec( query ));
  	for (result::const_iterator c = DOCS.begin(); c != DOCS.end(); ++c) {
		
		ID = c[0].as<int>();
		if(ID!=lastID){
			if(lastID!=0){
				xmlTextWriterStartElement(writer, BAD_CAST "DocumentTotals");
					sprintf(buf,"%4.2f",TaxPayable);
					xmlTextWriterWriteElement(writer, BAD_CAST "TaxPayable", BAD_CAST buf);
					sprintf(buf,"%4.2f",NetTotal);
					xmlTextWriterWriteElement(writer, BAD_CAST "NetTotal", BAD_CAST buf);
					sprintf(buf,"%4.2f",GrossTotal);
					xmlTextWriterWriteElement(writer, BAD_CAST "GrossTotal", BAD_CAST buf);
				xmlTextWriterEndElement(writer);
				
				rc = xmlTextWriterEndElement(writer); // Invoice
			}
				
			TaxPayable = c[4].as<int>()/100.0;
			NetTotal = c[5].as<int>()/100.0;
			GrossTotal = c[6].as<int>()/100.0;
				
			lineNum = 0;
			xmlTextWriterStartElement(writer, BAD_CAST "Invoice");
			
				pt =  c[8].as<string>().c_str();
				sprintf(buf2,"%s",pt);
				status = regexec(&re, buf2, 3, pm, 0);
				if(status==0){
					if(c[3].as<int>()==2)
						sprintf(buf, "VD %.*s/%.*s",pm[1].rm_eo-pm[1].rm_so, buf2+pm[1].rm_so, pm[2].rm_eo-pm[2].rm_so, buf2+pm[2].rm_so );
					else if (c[3].as<int>()==9)
						sprintf(buf, "NC %.*s/%.*s",pm[1].rm_eo-pm[1].rm_so, buf2+pm[1].rm_so, pm[2].rm_eo-pm[2].rm_so, buf2+pm[2].rm_so );
					else{
						printf("Inesperado!\n");
						exit(1);
					}
					//printf("deu!\n");
				}else{
					printf("Na deu! %s\n",pt);
					exit(1);
				}
			
				xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceNo", BAD_CAST buf);
				xmlTextWriterStartElement(writer, BAD_CAST "DocumentStatus");
					//2011-12-01 00:30:00.189 -> 2014-11-01T11:42:36
					xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceStatus", BAD_CAST c[10].as<string>().c_str());
			
					strptime(c[2].as<string>().c_str(), "%Y-%m-%d %H:%M:%S", &mtm);
					strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S", &mtm);
					xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceStatusDate", BAD_CAST buf);
					xmlTextWriterWriteElement(writer, BAD_CAST "SourceID", BAD_CAST "1");
					xmlTextWriterWriteElement(writer, BAD_CAST "SourceBilling", BAD_CAST "P");
				rc = xmlTextWriterEndElement(writer);
				
				xmlTextWriterWriteElement(writer, BAD_CAST "Hash", BAD_CAST c[1].as<string>().c_str());
				xmlTextWriterWriteElement(writer, BAD_CAST "HashControl", BAD_CAST c[11].as<string>().c_str());
				xmlTextWriterWriteElement(writer, BAD_CAST "Period", BAD_CAST c[9].as<string>().c_str());
			
				strptime(c[2].as<string>().c_str(), "%Y-%m-%d %H:%M:%S", &mtm);
				strftime(buf, sizeof buf, "%Y-%m-%d", &mtm);
				xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceDate", BAD_CAST buf);
				
				switch(c[3].as<int>()){
					case 2:  // Documento de Venda
						sprintf(buf,"%s","FT");
						nc=false;
						break;
					case 9: // Documento de Nota de Crédito
						sprintf(buf,"%s","NC");
						nc=true;
						break;
					default:
						sprintf(buf,"%d",c[3].as<int>());
						break;
				}
				
				xmlTextWriterWriteElement(writer, BAD_CAST "InvoiceType", BAD_CAST buf);
			
				xmlTextWriterStartElement(writer, BAD_CAST "SpecialRegimes");
					xmlTextWriterWriteElement(writer, BAD_CAST "SelfBillingIndicator", BAD_CAST "0");
					xmlTextWriterWriteElement(writer, BAD_CAST "CashVATSchemeIndicator", BAD_CAST "0");
					xmlTextWriterWriteElement(writer, BAD_CAST "ThirdPartiesBillingIndicator", BAD_CAST "0");
				rc = xmlTextWriterEndElement(writer);
			
				xmlTextWriterWriteElement(writer, BAD_CAST "SourceID", BAD_CAST "1");
				strptime(c[2].as<string>().c_str(), "%Y-%m-%d %H:%M:%S", &mtm);
				strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S", &mtm);
				xmlTextWriterWriteElement(writer, BAD_CAST "SystemEntryDate", BAD_CAST buf);
				xmlTextWriterWriteElement(writer, BAD_CAST "CustomerID", BAD_CAST c[12].as<string>().c_str());
			
			lastID=ID;
		}
		
		xmlTextWriterStartElement(writer, BAD_CAST "Line");
			sprintf(buf,"%d",++lineNum);
			xmlTextWriterWriteElement(writer, BAD_CAST "LineNumber", BAD_CAST buf);
			xmlTextWriterWriteElement(writer, BAD_CAST "ProductCode", BAD_CAST  c[13].as<string>().c_str());
			xmlTextWriterWriteElement(writer, BAD_CAST "ProductDescription", BAD_CAST c[14].as<string>().c_str());
			xmlTextWriterWriteElement(writer, BAD_CAST "Quantity", BAD_CAST c[15].as<string>().c_str());
			xmlTextWriterWriteElement(writer, BAD_CAST "UnitOfMeasure", BAD_CAST c[16].as<string>().c_str());
		
			Total = c[17].as<int>()/100.0;
			sprintf(buf,"%4.2f",Total);
			xmlTextWriterWriteElement(writer, BAD_CAST "UnitPrice", BAD_CAST buf);

			strptime(c[2].as<string>().c_str(), "%Y-%m-%d %H:%M:%S", &mtm);
			strftime(buf, sizeof buf, "%Y-%m-%d", &mtm);
			xmlTextWriterWriteElement(writer, BAD_CAST "TaxPointDate", BAD_CAST buf);

	



			if ( nc == true )
			{

				xmlTextWriterStartElement(writer, BAD_CAST "References");
				pt =  c[20].as<string>().c_str();
				sprintf(buf2,"%s",pt);
				status = regexec(&re, buf2, 3, pm, 0);
				if(status==0){
						sprintf(buf, "VD %.*s/%.*s",pm[1].rm_eo-pm[1].rm_so, buf2+pm[1].rm_so, pm[2].rm_eo-pm[2].rm_so, buf2+pm[2].rm_so );
					//printf("deu!\n");
				}else{
					printf("Na deu! %s\n",pt);
					exit(1);
				}
			

		
				xmlTextWriterWriteElement(writer, BAD_CAST "Reference", BAD_CAST buf);
				xmlTextWriterWriteElement(writer, BAD_CAST "Reason", BAD_CAST "Anulacao");
				xmlTextWriterEndElement(writer); // References
			}
			else
			
			if ( false ) // (c[20].as<string>() != "-1" )
			{
			
				xmlTextWriterStartElement(writer, BAD_CAST "OrderReferences");
				
				/* xmlTextWriterStartElement(writer, BAD_CAST "References");
				printf("Fatura:");	
				pt =  c[20].as<string>().c_str();
				printf("\t%s\t", pt);
				pt =  c[21].as<string>().c_str();
				printf("%s\n", pt); */
				/* References
			 	*   FATURA:
  			 	*     <OriginatingON>VC 5315</OriginatingON>
                         	*   <OrderDate>2014-05-01</OrderDate>
                         	*/
				
				pt =  c[20].as<string>().c_str();
				sprintf(buf2,"%s",pt);
				status = regexec(&re, buf2, 3, pm, 0);
				if(status==0){
				 		// sprintf(buf, "VD %.*s/%.*s",pm[1].rm_eo-pm[1].rm_so, buf2+pm[1].rm_so, pm[2].rm_eo-pm[2].rm_so, buf2+pm[2].rm_so );
				 		sprintf(buf, "VD %.*s/%.*s",pm[1].rm_eo-pm[1].rm_so, buf2+pm[1].rm_so, pm[2].rm_eo-pm[2].rm_so, buf2+pm[2].rm_so );
					//printf("deu!\n");
				}else{
					printf("Na deu! %s\n",pt);
					exit(1);
				}
				
				xmlTextWriterWriteElement(writer, BAD_CAST "OriginatingON", BAD_CAST buf);
				xmlTextWriterWriteElement(writer, BAD_CAST "OrderDate", BAD_CAST c[21].as<string>().c_str());
				xmlTextWriterEndElement(writer); // References

			}

			xmlTextWriterWriteElement(writer, BAD_CAST "Description", BAD_CAST c[14].as<string>().c_str());
		
			Total *= c[15].as<int>();
			sprintf(buf,"%4.2f",Total);
			
			if ( nc == false )
			{
			xmlTextWriterWriteElement(writer, BAD_CAST "CreditAmount", BAD_CAST buf);
			}
			else 
			xmlTextWriterWriteElement(writer, BAD_CAST "DebitAmount", BAD_CAST buf);

		
			IVA = c[19].as<int>();
			if(IVA>1700)
				sprintf(buf,"%s","NOR");
			else if (IVA>1000)
				sprintf(buf,"%s","INT");
			else if(IVA>0)
				sprintf(buf,"%s","RED");
			else
				sprintf(buf,"%s","ISE");
		
			xmlTextWriterStartElement(writer, BAD_CAST "Tax");
				xmlTextWriterWriteElement(writer, BAD_CAST "TaxType", BAD_CAST "IVA");
				xmlTextWriterWriteElement(writer, BAD_CAST "TaxCountryRegion", BAD_CAST "PT");
				xmlTextWriterWriteElement(writer, BAD_CAST "TaxCode", BAD_CAST buf);
			
				Total = IVA/100.0;
				sprintf(buf,"%4.2f",Total);
			
				xmlTextWriterWriteElement(writer, BAD_CAST "TaxPercentage", BAD_CAST buf);
			
			rc = xmlTextWriterEndElement(writer);
		
		
		rc = xmlTextWriterEndElement(writer);
		
		
		
		
			
			
  	}
	
	if(lastID!=0){
		xmlTextWriterStartElement(writer, BAD_CAST "DocumentTotals");
			sprintf(buf,"%4.2f",TaxPayable);
			xmlTextWriterWriteElement(writer, BAD_CAST "TaxPayable", BAD_CAST buf);
			sprintf(buf,"%4.2f",NetTotal);
			xmlTextWriterWriteElement(writer, BAD_CAST "NetTotal", BAD_CAST buf);
			sprintf(buf,"%4.2f",GrossTotal);
			xmlTextWriterWriteElement(writer, BAD_CAST "GrossTotal", BAD_CAST buf);
		xmlTextWriterEndElement(writer);
		
		rc = xmlTextWriterEndElement(writer); // Invoice
	}
	
}



/* #####   EOF ####################################################################### */

