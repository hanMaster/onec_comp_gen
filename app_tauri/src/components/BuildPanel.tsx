import { Button, Flex, Input, Notification } from "@mantine/core";
import useStore from "../store";
import { invoke } from "@tauri-apps/api/core";
import React, { useEffect, useState } from "react";

export default function BuildPanel() {
  const [buildPath, setBuildPath] = useState<string>("");
  const [return_build, setReturn_build] = React.useState<String>("");

  async function build() {
    let res = (await invoke("build", {
      path: buildPath,
      state: JSON.stringify({
        methods: [...useStore.getState().methods],
        props: [...useStore.getState().props],
      }),
    })) as String;

    console.log(res);

    setReturn_build(res);
  }

  useEffect(() => {
    if (!buildPath) {
      invoke<string>("current_dir").then((path) => {
        setBuildPath(path);
      })
    }
  }, []);

  return (
    <>
      {!!return_build && (
        <Notification
          onClose={() => setReturn_build("")}
          closeButtonProps={{ "aria-label": "Hide notification" }}
        >
          {return_build}
        </Notification>
      )}

      <Flex direction={"column"} gap={10}>
        <Input.Wrapper label="Путь к проекту">
          <Input
            placeholder="Путь к проекту"
            value={buildPath}
            onChange={(e) => setBuildPath(e.target.value)}
          ></Input>
        </Input.Wrapper>
        <Button onClick={build}>Собрать</Button>
      </Flex>
    </>
  );
}
