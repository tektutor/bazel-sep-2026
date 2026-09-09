package org.tektutor;

import static org.junit.Assert.assertEquals;
import org.junit.Test;

public class HelloTest {

	@Test
	public void sayHelloTest() {
		Hello hello = new Hello();

		String expectedResult = "Hello, Bazel !";
		String actualResult = hello.sayHello();

		assertEquals( expectedResult, actualResult );
	}

}
