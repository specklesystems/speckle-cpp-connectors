#! /bin/bash

echo ${BASE_NAME}
echo targetName="${BASE_NAME}"
targetName="${PRODUCT_NAME}.${WRAPPER_EXTENSION}"
target="${SYMROOT}/${CONFIGURATION}/${PRODUCT_NAME}.${WRAPPER_EXTENSION}"

echo "Signing: $targetName"
/usr/bin/codesign --force --sign "${DEV_SIGNATURE}" --timestamp --requirements =designated\ =\>\ anchor\ apple\ generic\ \ and\ identifier\ \"\$self.identifier\"\ and\ \(\(cert\ leaf\[field.1.2.840.113635.100.6.1.9\]\ exists\)\ or\ \(\ certificate\ 1\[field.1.2.840.113635.100.6.2.6\]\ exists\ and\ certificate\ leaf\[field.1.2.840.113635.100.6.1.13\]\ exists\ \ and\ certificate\ leaf\[subject.OU\]\ =\ \"T4C464595P\"\ \)\) "$target"

echo "Notarising: $targetName"

tempfile="/private/tmp/notarizationinfo.txt"

echo "Compressing bundle… $target"
/usr/bin/ditto -c -k --keepParent "$target" "${BASE_NAME}.zip"

#notarise:
echo "Submitting compressed bundle for notarization…"

xcrun altool --notarize-app --verbose --primary-bundle-id "${PRODUCT_BUNDLE_IDENTIFIER}" --username "${AC_USERNAME}" --team-id "${TEAM_ID}" --password "@keychain:${AC_PASSWORD}" --file "${BASE_NAME}.zip" &> "$tempfile"

retVal=$?
if [ $retVal -ne 176 ]; then #176 means the package was already uploaded
    uuid=$( cat "$tempfile" | grep RequestUUID )
    if [ -z "$uuid" ]; then
		echo "error: Failed to upload add-on"
		cat "$tempfile"
		rm "$tempfile"
		exit 1
    fi

    rm "$tempfile"

    echo "Waiting for result from Apple…"

    uuid=${uuid#"RequestUUID = "}
    echo "RequestUUID: $uuid"

    #wait for finish:
    retVal=0
    while [ $retVal -eq 0 ] ; do
        echo "Waiting for Apple: $uuid"
        sleep 30
        xcrun altool --notarization-info "$uuid" -u "${AC_USERNAME}" -p "@keychain:${AC_PASSWORD}" 2>&1 | grep progress &>/dev/null
        retVal=$?
    done

    #staple:
    echo "Stapling…"
    xcrun stapler staple "$target"
    retVal=$?

    echo "…done!"
fi

rm "${BASE_NAME}.zip"

exit 0


