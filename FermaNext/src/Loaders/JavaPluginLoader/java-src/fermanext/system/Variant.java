
package fermanext.system;

//import org.apache.xindice.xml.TextWriter;

import org.w3c.dom.Document;
import org.w3c.dom.DocumentFragment;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

/**
 * Variant is a generic type that can be used to store any type of
 * Xindice variable.  Variants will do their best to convert the value
 * that they store to any requested type.
 */
public final class Variant {

    private Object value = null;
    private int type = -1;


    public Variant() {
    }

    public Variant(Variant value) {
        this.value = value.value;
        type = value.type;
    }

    public Variant(String value) {
        this.value = value;
        type = VariantTypes.STRING;
    }

    public Variant(boolean value) {
        this.value = new Boolean(value);
        type = VariantTypes.BOOLEAN;
    }

    public Variant(byte value) {
        this.value = new Byte(value);
        type = VariantTypes.BYTE;
    }

    public Variant(char value) {
        this.value = new Character(value);
        type = VariantTypes.CHAR;
    }

    public Variant(short value) {
        this.value = new Short(value);
        type = VariantTypes.SHORT;
    }

    public Variant(int value) {
        this.value = new Integer(value);
        type = VariantTypes.INT;
    }

    public Variant(long value) {
        this.value = new Long(value);
        type = VariantTypes.LONG;
    }

    public Variant(float value) {
        this.value = new Float(value);
        type = VariantTypes.FLOAT;
    }

    public Variant(double value) {
        this.value = new Double(value);
        type = VariantTypes.DOUBLE;
    }

    public Variant(Element value) {
        this.value = value;
        type = VariantTypes.ELEMENT;
    }

    public Variant(Document value) {
        this.value = value;
        type = VariantTypes.DOCUMENT;
    }

    public Variant(DocumentFragment value) {
        this.value = value;
        type = VariantTypes.DOCUMENTFRAGMENT;
    }

    public Variant(Object value) {
        this.value = value;
        type = VariantTypes.OBJECT;
    }

    /**
     * getType returns the type of the variable being stored in the Variant.
     *
     * @return The variable type
     */
    public int getType() {
        return type;
    }

    /**
     * isNull returns whether or not the variable is storing a null value.
     */
    public boolean isNull() {
        return value == null;
    }

    /**
     * set sets the value as a String.
     *
     * @param value The value
     */
    public void set(String value) {
        this.value = value;
        type = VariantTypes.STRING;
    }

    /**
     * set sets the value as a boolean.
     *
     * @param value The value
     */
    public void set(boolean value) {
        this.value = new Boolean(value);
        type = VariantTypes.BOOLEAN;
    }

    /**
     * set sets the value as a byte.
     *
     * @param value The value
     */
    public void set(byte value) {
        this.value = new Byte(value);
        type = VariantTypes.BYTE;
    }

    /**
     * set sets the value as a char.
     *
     * @param value The value
     */
    public void set(char value) {
        this.value = new Character(value);
        type = VariantTypes.CHAR;
    }

    /**
     * set sets the value as a short
     *
     * @param value The value
     */
    public void set(short value) {
        this.value = new Short(value);
        type = VariantTypes.SHORT;
    }

    /**
     * set sets the value as an int.
     *
     * @param value The value
     */
    public void set(int value) {
        this.value = new Integer(value);
        type = VariantTypes.INT;
    }

    /**
     * set sets the value as a long.
     *
     * @param value The value
     */
    public void set(long value) {
        this.value = new Long(value);
        type = VariantTypes.LONG;
    }

    /**
     * set sets the value as a float.
     *
     * @param value The value
     */
    public void set(float value) {
        this.value = new Float(value);
        type = VariantTypes.FLOAT;
    }

    /**
     * set sets the value as a double.
     *
     * @param value The value
     */
    public void set(double value) {
        this.value = new Double(value);
        type = VariantTypes.DOUBLE;
    }

    /**
     * set sets the value as an Element.
     *
     * @param value The value
     */
    public void set(Element value) {
        this.value = value;
        type = VariantTypes.ELEMENT;
    }

    /**
     * set sets the value as a Document.
     *
     * @param value The value
     */
    public void set(Document value) {
        this.value = value;
        type = VariantTypes.DOCUMENT;
    }

    /**
     * set sets the value as a DocumentFragment.
     *
     * @param value The value
     */
    public void set(DocumentFragment value) {
        this.value = value;
        type = VariantTypes.DOCUMENTFRAGMENT;
    }

    /**
     * set sets the value as an Object.
     *
     * @param value The value
     */
    public void set(Object value) {
        this.value = value;
        this.type = this.value != null ? VariantTypes.OBJECT
                : VariantTypes.EMPTY;
    }

    /**
     * getString returns the value as a String.
     *
     * @return The value
     */
    public String getString() {
        if (type == VariantTypes.STRING) {
            return (String) value;
        } else {
            return value.toString();
        }
    }

    /**
     * getBoolean returns the value as a boolean.
     *
     * @return The value
     */
    public boolean getBoolean() {
        if (type == VariantTypes.BOOLEAN) {
            return ((Boolean) value).booleanValue();
        } else {
            return "[true][yes][1][y][on]".indexOf("[" + value.toString().toLowerCase() + "]") != -1;
        }
    }

    /**
     * getByte returns the value as a byte.
     *
     * @return The value
     */
    public byte getByte() {
        if (type == VariantTypes.BYTE) {
            return ((Byte) value).byteValue();
        } else {
            return Byte.parseByte(value.toString());
        }
    }

    /**
     * getChar returns the value as a char.
     *
     * @return The value
     */
    public char getChar() {
        if (type == VariantTypes.CHAR) {
            return ((Character) value).charValue();
        } else {
            return value.toString().charAt(0);
        }
    }

    /**
     * getShort returns the value as a short.
     *
     * @return The value
     */
    public short getShort() {
        if (type == VariantTypes.SHORT) {
            return ((Short) value).shortValue();
        } else {
            return Short.parseShort(value.toString());
        }
    }

    /**
     * getInt returns the value as an int.
     *
     * @return The value
     */
    public int getInt() {
        if (type == VariantTypes.INT) {
            return ((Integer) value).intValue();
        } else {
            return Integer.parseInt(value.toString());
        }
    }

    /**
     * getLong returns the value as a long.
     *
     * @return The value
     */
    public long getLong() {
        if (type == VariantTypes.LONG) {
            return ((Long) value).longValue();
        } else {
            return Long.parseLong(value.toString());
        }
    }

    /**
     * getFloat returns the value as a float.
     *
     * @return The value
     */
    public float getFloat() {
        if (type == VariantTypes.FLOAT) {
            return ((Float) value).floatValue();
        } else {
            return Float.parseFloat(value.toString());
        }
    }

    /**
     * getDouble returns the value as a double.
     *
     * @return The value
     */
    public double getDouble() {
        if (type == VariantTypes.DOUBLE) {
            return ((Double) value).doubleValue();
        } else {
            return Double.parseDouble(value.toString());
        }
    }

    /**
     * getElement returns the value as an Element.
     *
     * @return The value
     */
    public Element getElement() {
        if (type == VariantTypes.ELEMENT) {
            return (Element) value;
        } else {
            return null;
        }
    }

    /**
     * getDocument returns the value as a Document.
     *
     * @return The value
     */
    public Document getDocument() {
        if (type == VariantTypes.DOCUMENT) {
            return (Document) value;
        } else {
            return null;
        }
    }

    /**
     * getDocumentFragment returns the value as a DocumentFragment.
     *
     * @return The value
     */
    public DocumentFragment getDocumentFragment() {
        if (type == VariantTypes.DOCUMENTFRAGMENT) {
            return (DocumentFragment) value;
        } else {
            return null;
        }
    }

    /**
     * getObject returns the value as an Object.
     *
     * @return The value
     */
    public Object getObject() {
        return value;
    }

    /**
     * toString returns the value as a Stringified representation.
     *
     * @return The stringified value
     */
    public String toString() {
        switch (type) {

            case VariantTypes.ELEMENT:
            case VariantTypes.DOCUMENT:
            case VariantTypes.DOCUMENTFRAGMENT: 
               //TODO: should be changed on some smart Stringified representation.
               assert false;
            default:
                return value.toString();
        }
    }
}

