
package fermanext.system;

import fermanext.logging.log4cxx.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.DocumentFragment;
import org.w3c.dom.Element;

/**
 * VariantTypes defines identifying values for the types that Xindice
 * Reflectable methods support.
 */
public final class VariantTypes {

    /**
     * Logger used by this class
     */
    private static Logger logger = Logger.getLogger("fermanext.system.VariantTypes");

    public final static int EMPTY = -1;
    public final static int UNKNOWN = -1;
    public final static int VOID = 0;
    public final static int BOOLEAN = 1;
    public final static int BYTE = 2;
    public final static int CHAR = 3;
    public final static int SHORT = 4;
    public final static int INT = 5;
    public final static int LONG = 6;
    public final static int FLOAT = 7;
    public final static int DOUBLE = 8;
    public final static int OBJECT = 10;
    public final static int STRING = 11;
    public final static int ELEMENT = 12;
    public final static int DOCUMENT = 13;
    public final static int DOCUMENTFRAGMENT = 14;
    public final static int ARGS = 15;
    public final static int VARIANT = 20;

    private VariantTypes () {
    }

    /**
     * TypeOf inspects the Class, and returns the type value for that Class
     * or UNKNOWN, if the Class is not an acceptable signature type.
     *
     * @param c The Class to check
     * @return The type value
     */
    public static int typeOf(Class c) {
        if (c.isArray()) {
            c = c.getComponentType();
        }

        if (c == void.class) {
            return VOID;
        }

        if (c.isPrimitive()) {
            if (c == Boolean.TYPE) {
                return BOOLEAN;
            } else if (c == Byte.TYPE) {
                return BYTE;
            } else if (c == Character.TYPE) {
                return CHAR;
            } else if (c == Short.TYPE) {
                return SHORT;
            } else if (c == Integer.TYPE) {
                return INT;
            } else if (c == Long.TYPE) {
                return LONG;
            } else if (c == Float.TYPE) {
                return FLOAT;
            } else if (c == Double.TYPE) {
                return DOUBLE;
            }
        } else {
            if (c == String.class) {
                return STRING;
            } else if (c == Variant.class) {
                return VARIANT;
            } else if (Element.class.isAssignableFrom(c)) {
                return ELEMENT;
            } else if (Document.class.isAssignableFrom(c)) {
                return DOCUMENT;
            } else if (DocumentFragment.class.isAssignableFrom(c)) {
                return DOCUMENTFRAGMENT;
            }
        }
        return UNKNOWN;
    }

    public static int typeOf(Object object) {
        return object != null ? typeOf(object.getClass())
                : EMPTY;
    }

    public static String typeName(int type) {
        switch (type) {

            case UNKNOWN:
                return "Unknown";
            case VOID:
                return "void";
            case BOOLEAN:
                return "boolean";
            case BYTE:
                return "byte";
            case CHAR:
                return "char";
            case SHORT:
                return "short";
            case INT:
                return "int";
            case LONG:
                return "long";
            case FLOAT:
                return "float";
            case DOUBLE:
                return "double";
            case OBJECT:
                return "Object";
            case STRING:
                return "String";
            case ELEMENT:
                return "Element";
            case DOCUMENT:
                return "Document";
            case DOCUMENTFRAGMENT:
                return "DocumentFragment";
            case ARGS:
                return "Args";
            case VARIANT:
                return "Variant";
            default:
                    logger.warn("invalid type found : " + type);
        }
        return "Invalid";
    }
}

