# Inage Diff

Warning. This is an alpha version of the application, which is actively under development, and bugs may be present!

This application allows you to work with 8-bit-per-channel RGB images. It supports PNG image files. Users can open two images and compare them by switching between them using the S key. Additionally, various image processing algorithms and automatic comparison tools based on specific metrics can be applied.

The "Comparators" menu contains a list of comparator algorithms. These algorithms perform automatic comparisons of the two loaded images and provide results either as text or as a third generated image. For example, we can compare both images to determine which one has a higher average brightness.  

The "Filters" menu contains a list of filters algorithms. After applying a filter, the S key toggles between the two resulting images produced by the filter algorithm. For instance, we can display only the Red channel, and now the images being compared will contain only that channel.
Filter algorithms are applied only to the two images being compared. They cannot be applied to the images resulting from the comparator's work. But the comparators will use these transformed images as a source for their work until the original compared images are restored using the menu option **View -> Show Original Image**.

To remove all transformations and return to the original images, select **View -> Show Original Image**.

If you apply a chain of filters, each subsequent filter is applied to the result of the previous one.
