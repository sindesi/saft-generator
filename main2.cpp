#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

#include <libxml/xmlwriter.h>
#define MY_ENCODING "Windows-1252"

		connection C("dbname=koncepto user=postgres hostaddr=127.0.0.1 port=5432");




int main(void){


	int OpenPSQL ();
	
	xmlTextWriterPtr writer;
	int rc=0;
	printf("A inicializar: %d",rc);
	
	nontransaction N(C);
	
	// Open file 
	writer = xmlNewTextWriterFilename("teste.xml",0);
	// Set indentation 
	xmlTextWriterSetIndent(writer, 4);
	
	
	rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
	rc = xmlTextWriterStartElement(writer, BAD_CAST "AuditFile");


	rc = xmlTextWriterStartElement(writer, BAD_CAST "Header");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "AuditFileVersion", BAD_CAST "1.03_01");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "CompanyID", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "TaxRegistrationNumber", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "TaxAccountingBasis", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "CompanyName", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "CompanyAddress", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "FiscalYear", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "StartDate", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "EndDate", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "CurrencyCode", BAD_CAST "EUR");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "DateCreated", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "TaxEntity", BAD_CAST "");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "ProductCompanyTaxID", BAD_CAST "508476879");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "SoftwareCertificateNumber", BAD_CAST "1183");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "ProductID", BAD_CAST "Koncepto/SINDESI SISTEMAS E SOLUCOES INFORMATICAS LDA");
	rc = xmlTextWriterWriteElement(writer, BAD_CAST "ProductVersion", BAD_CAST "4.3.4");
	rc = xmlTextWriterEndElement(writer); // Header

	rc = xmlTextWriterStartElement(writer, BAD_CAST "MasterFiles");

	void LoadCustomers ();
	void LoadProducts ();
	void LoadTaxes ();
	

	
	
	
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
	
/*	SELECT 
        d.doc_number
        , c.client_account as antes
        , cl.id as depois
        , ent.id as entityID
        ,ent.tax_number
FROM documents d
INNER JOIN cashflow c
        ON d.id=c.reference_document_id AND c.doc_type = 0 AND c.payment_method = 7
INNER JOIN clientcard cc
        ON c.client_account=cc.id
INNER JOIN client cl
        ON cc.clientid=cl.id
INNER JOIN entity ent
        ON cl.entityid=ent.id
WHERE d.doc_number='VB152761'; */

	//rc = xmlTextWriterWriteAttribute(writer,"version","1.0");
	rc = xmlTextWriterEndElement(writer);//SalesInvoices
	rc = xmlTextWriterEndElement(writer);//SourceDocuments
	rc = xmlTextWriterEndElement(writer);
	xmlFreeTextWriter(writer);

		
	/*
	 * Close PSQL Connection 
	 */
	C.disconnect ();
}


void OpenPSQL (void)
{
	/*
	 * PSQL Connection 
	 */
	//try{
		// connection C("dbname=koncepto user=postgres hostaddr=127.0.0.1 port=5432");
		if (C.is_open()) {
			cout << "Opened database successfully: " << C.dbname() << endl;
		} else {
			cout << "Can't open database" << endl;
			return ;
		}
	/*}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}*/
}


void LoadCustomers (void)
{
	

	char sql[]="SELECT " \
		" 			c.id AS client_id, "\
        "			CASE  "\
         "       		WHEN LENGTH(COALESCE(TRIM(e.name), 'Desconhecido')) = 0 THEN 'Desconhecido'"\
          "      		ELSE COALESCE(TRIM(e.name), 'Desconhecido')"\
        "			END AS client_name, "\
        "			CASE  "\
        "        	WHEN LENGTH(COALESCE(TRIM(e.address), 'Desconhecido')) = 0 THEN 'Desconhecido'"\
		"			ELSE COALESCE(TRIM(e.address), 'Desconhecido')"\
        " 			END AS client_address, "\
        "			COALESCE(TRIM(e.postal_code), '') AS client_zipcode,"\
        "			COALESCE(TRIM(e.phone), '') AS client_phone,"\
        "			COALESCE(TRIM(clientcards.name), COALESCE(TRIM(e.tax_number), '999999990')) AS client_taxnumber"\
        " FROM "\
        "client c        LEFT OUTER JOIN         (               SELECT                  cc.clientid AS clientid, max(cc.name) AS NAME           FROM                    clientcard cc           GROUP BY                        cc.clientid     ) clientcards ON                clientcards .clientid = c.id    LEFT OUTER JOIN entity e "\
		"ON             e.id = c.entityid ORDER BY      c.id";

	
	result R( N.exec( sql ));

    /* <Customer>
      <CustomerID>Consumidor Final</CustomerID>
      <AccountID>Desconhecido</AccountID>
      <CustomerTaxID>999999990</CustomerTaxID>
      <CompanyName>Consumidor Final</CompanyName>
      <BillingAddress>
        <AddressDetail>Desconhecido</AddressDetail>
        <City>Desconhecido</City>
        <PostalCode>Desconhecido</PostalCode>
        <Country>Desconhecido</Country>
      </BillingAddress>
      <SelfBillingIndicator>0</SelfBillingIndicator>
    </Customer> 
	 */
	
	//rc = xmlTextWriterStartElement(writer, BAD_CAST "Customer");
	for (result::const_iterator c = R.begin(); c != R.end(); ++c) {

		xmlTextWriterStartElement(writer, BAD_CAST "Customer");
		xmlTextWriterWriteElement(writer, BAD_CAST "CustomerID", BAD_CAST c[0].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "CustomerTaxID", BAD_CAST c[5].as<string>().c_str());		
		xmlTextWriterWriteElement(writer, BAD_CAST "CompanyName", BAD_CAST c[1].as<string>().c_str());
		xmlTextWriterStartElement(writer, BAD_CAST "BillingAddress");
		xmlTextWriterWriteElement(writer, BAD_CAST "AddressDetail", BAD_CAST "Desconhecido");
		xmlTextWriterWriteElement(writer, BAD_CAST "City", BAD_CAST c[2].as<string>().c_str());
		xmlTextWriterWriteElement(writer, BAD_CAST "PostalCode", BAD_CAST "Desconhecido");
		xmlTextWriterWriteElement(writer, BAD_CAST "Country", BAD_CAST "PT");
		xmlTextWriterEndElement(writer);	//BillingAddress
		xmlTextWriterWriteElement(writer, BAD_CAST "SelfBillingIndicator", BAD_CAST "0");
		xmlTextWriterEndElement(writer);//Customer
				
	}

}


void LoadProducts (void)
{
		
	
	/* <Product>
		<ProductType>P</ProductType>
		<ProductCode>5</ProductCode>
		<ProductDescription>garoto</ProductDescription>
		<ProductNumberCode>5</ProductNumberCode>
	</Product>
	*/

	char products[]="SELECT id, description FROM products WHERE product_type NOT IN (-5, -4, 6);";
	
	
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
	

void LoadTaxes (void);
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
	
		char taxes[]="SELECT DISTINCT "\
        "       CAST('IVA' AS CHAR(3)) AS TaxType, "\
        "       CAST('PT' AS CHAR(2)) AS TaxCountryRegion,   "\
	"       CAST(  "\
	"		CASE    "\
	"                  WHEN (p.Tax1/100) = 0 THEN 'ISE'   "\
	"		   WHEN (p.Tax1/100) IN (5, 6, 7) THEN 'RED'  "\
	"		   WHEN (p.Tax1/100) IN (11, 12, 13) THEN 'INT'   "\
	"		   WHEN (p.Tax1/100) IN (19, 20, 21, 22, 23) THEN 'NOR' "\
	"		ELSE 'OUT'       "\
	"	END AS CHAR(3)  )  "\
	"	AS TaxCode, CAST(('IVA ' || (p.Tax1/100) || '%')  "\
	"	AS VARCHAR(255)) AS Description,  "\
	"	CAST((p.Tax1/100) AS DECIMAL(18,2)) AS TaxPercentage  "\
	"	FROM  products p ORDER BY   5; ";
	
		
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
	
}