package test.sslclient;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.util.Map;

import javax.net.ssl.SSLContext;

import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.conn.ssl.NoopHostnameVerifier;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.conn.ssl.TrustSelfSignedStrategy;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.ssl.SSLContexts;
import org.apache.http.util.EntityUtils;


public class Main {
	private static String keyPassword;
	private static String keyStorePath = "ihmCA.keystore";
	public static SSLContext custom(){
		SSLContext ssl = null;
		FileInputStream stream = null;
		KeyStore keystore = null;
		
		try {
			ssl = SSLContexts.custom().loadTrustMaterial(new File(keyStorePath), null, new TrustSelfSignedStrategy()).build();
				
		} catch (CertificateException | IOException | KeyManagementException | NoSuchAlgorithmException | KeyStoreException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return ssl;
	}
	
	public static void send(String url, Map<String, Object> map, String encoding) throws ClientProtocolException, IOException {
		SSLContext sslContext = custom();
		SSLConnectionSocketFactory sslsf = new SSLConnectionSocketFactory(
				sslContext,	NoopHostnameVerifier.INSTANCE);
		CloseableHttpClient httpClient = HttpClients.custom().setSSLSocketFactory(sslsf).build();
		HttpGet httpGet = new HttpGet("https://" + url);

		CloseableHttpResponse response = httpClient.execute(httpGet);
		String responseBody = EntityUtils.toString(response.getEntity(), "utf-8");
		System.out.println(responseBody);
		
	}
	public static void main(String[] args) {
		String url = "100.2.52.45:8443/RmbDisk/rmbDisk";
		try {
			send(url, null, null);
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}