
package fermanext.system;

public interface NativeObject extends UUIDObject
{
    /** 
     * This method should check if derived native class is valid and can be 
     * natively casted to instance, from which it was created 
     */
    public boolean isValid ();
}
