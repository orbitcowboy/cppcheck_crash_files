void dummy() {
  NSArray* loginItemsArray = static_cast<NSArray*>(LSSharedFileListCopySnapshot(loginItems, NULL));
  for (NSUInteger i = 0; i < [loginItemsArray count]; ++i) {
    if (LSSharedFileListItemResolve(itemRef, 0, static_cast<CFURLRef*>(&url), NULL) == noErr)
      return;
  }
  [loginItemsArray release];
}
