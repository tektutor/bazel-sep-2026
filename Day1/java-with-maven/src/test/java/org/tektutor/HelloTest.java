package org.tektutor;

import org.junit.Test;
import org.junit.After;
import org.junit.Before;
import static org.junit.Assert.*;

public class HelloTest {

	private Hello hello;
	private String expectedResponse;
	private String actualResponse;

	@Before
	public void initialize() {
		hello = new Hello();
		expectedResponse = new String();
		actualResponse = new String();
	}

	@Test
	public void testSayHello() {
		actualResponse = hello.sayHello();
		expectedResponse = "Hello Java!";

		assertEquals ( expectedResponse, actualResponse );
	}

	@After
	public void cleanUp() {
		hello = null;
		expectedResponse = null;
		actualResponse = null; 
	}

}
