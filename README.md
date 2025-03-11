# User Guide for the Application

Warning. This is an alpha version of the application, which is actively under development, and bugs may be present!

This application allows you to work with 8-bit-per-channel RGB images. It supports PNG image files. Users can open two images and compare them by switching between them using the S key. Additionally, various image processing algorithms and automatic comparison tools based on specific metrics can be applied.

## Menu Description

### **File Menu**
- **File -> Open Images**  
  Open two images for comparison.
  
- **File -> Close Image**  
  Close the currently opened images.
  
- **File -> Save Image As**  
  Create a duplicate of the current image and save it as a new file.
  
- **File -> Save Visible Area As**  
  Create a duplicate of the currently visible area and save it as a new file.

### **Image Analysis Menu**
- **Image Analysis -> Switch Between Images Being Compared**  
  Toggle between the two images being compared using this option or the S key.

- **Image Analysis -> Color Picker**  
  Open or close the Color Picker tool.

### **Comparators Menu**
The "Comparators" menu contains a list of comparator algorithms. These algorithms perform automatic comparisons of the two loaded images and provide results either as text or as a third generated image. For example, we can compare both images to determine which one has a higher average brightness.  

### **Transformers Menu**
The "Transformers" menu contains a list of transformer algorithms. After applying a transformer, the S key toggles between the two resulting images produced by the transformer algorithm. For instance, we can display only the Red channel, and now the images being compared will contain only that channel.
Transformer algorithms are applied only to the two images being compared. They cannot be applied to the images resulting from the comparator's work. But the comparators will use these transformed images as a source for their work until the original compared images are restored using the menu option **Transformers -> Show Original Image**..

To remove all transformations and return to the original images, select **Transformers -> Show Original Image**.

If you apply a chain of transformer algorithms, each subsequent algorithm is applied to the result of the previous one.
