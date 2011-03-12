#include <stdio.h>
#include <jni.h>
#include <string.h>

static JNIEnv * JAVAENV = 0;
static JavaVM * JAVAVM = 0;
static string previousClassName;
static string previousMethodName;
static jclass previousClass = NULL;
static jmethodID previousMethod = NULL;

JNIEnv* create_java_vm(JavaVM ** jvm)
{
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    JavaVMOption options;
    options.optionString = "-Djava.class.path=."; //Path to the java source code
    vm_args.version = JNI_VERSION_1_6; //JDK version. This indicates version 1.6
    vm_args.nOptions = 1;
    vm_args.options = &options;
    vm_args.ignoreUnrecognized = 0;
    
    int ret = JNI_CreateJavaVM(jvm, (void**)&env, &vm_args);
    if(ret < 0) return 0; //error
    return env;
}

extern "C"
{
	TCAPIEXPORT int initialize()
	{
	    JAVAENV = create_vm(&JAVAVM);
	    if (JAVAENV == NULL) return 0;
	}

	TCAPIEXPORT int exec(const char * classname, const char * methodname, const char * arg)
	{
	    if (JAVAENV == NULL) return 0;

	    jclass currentClass = NULL;
	    jmethodID currentMethod = NULL;
	    string currentClassName(classname);
	    string currentMethodName(methodname);

	    if ((previousClassName == currentClassName) &&
		(previousClass != NULL))
	    {
		currentClass = previousClass;
		if ((previousMethodName == currentMethodName) &&
		    (previousMethod != NULL))
		    currentMethod = previousMethod;
	    }
	    
	    //Obtaining Classes
	    if (currentClass == NULL)
		currentClass = JAVAENV->FindClass(classname);
	    
	    //Obtaining Method IDs
	    if (currentClass != NULL && currentMethod == NULL)
		currentMethod = env->GetStaticMethodID(currentClass,methodname,"(Ljava/lang/String;)V");

	    if (currentClass != NULL && currentMethod != NULL)
	    {
		jstring StringArg = env->NewStringUTF("\nabcdefg\n");
		//Calling another static method and passing string type parameter
		env->CallStaticVoidMethod(currentClass,currentMethod,StringArg);
		return 1;
	    }

	    return 0;
	}

	TCAPIEXPORT int finalize()
	{
	    int n = jvm->DestroyJavaVM();
	    return 1;
	}
}

