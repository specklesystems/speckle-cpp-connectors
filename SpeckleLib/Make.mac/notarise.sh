#! /bin/bash

echo ${BASE_NAME}
echo targetName="${BASE_NAME}"
targetName="${PRODUCT_NAME}.${WRAPPER_EXTENSION}"
target="${SYMROOT}/${CONFIGURATION}/${PRODUCT_NAME}.${WRAPPER_EXTENSION}"

echo "Signing: $targetName"

/usr/bin/codesign --force --sign "${DEV_SIGNATURE}" --timestamp  "$target"

echo "Notarising: $targetName"

tempfile="/private/tmp/notarizationinfo.txt"

echo "Compressing bundle… $target"
/usr/bin/ditto -c -k --keepParent "$target" "${BASE_NAME}.zip"

#notarise:
echo "Submitting compressed bundle for notarization…"

xcrun notarytool submit --wait --apple-id "${DEV_EMAIL}" --team-id "$(TEAM_ID}" --password "${DEV_PASSWORD}" "${BASE_NAME}.zip" &> "$tempfile"

echo "Submitted"

retVal=$?

uuid=$( cat "$tempfile" | grep -m 1 id: )
if [ -z "$uuid" ]; then
	echo "error: Failed to upload add-on"
	cat "$tempfile"
	rm "$tempfile"
	exit 1
fi

rm "$tempfile"

uuid=${uuid#"  id: "}
echo "RequestUUID: $uuid"

echo "Getting log from Apple…"
xcrun notarytool log --apple-id "${DEV_EMAIL}" --team-id "$(TEAM_ID}" --password "${DEV_PASSWORD}" "$uuid" 2>&1

retVal=$?

#staple:
echo "Stapling…"
xcrun stapler staple "$target"
retVal=$?

echo "…done!"

rm "${BASE_NAME}.zip"

exit 0


